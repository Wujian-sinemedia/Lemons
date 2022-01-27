/*
 ======================================================================================

 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.

 ======================================================================================
 */

namespace lemons::dsp::psola
{

template <typename SampleType>
void PeakFinder<SampleType>::clearAllArrays (bool free)
{
	if (free)
	{
		for (auto* array : arrays)
			array->clear();
	}
	else
	{
		for (auto* array : arrays)
			array->clearQuick();
	}
}

template <typename SampleType>
void PeakFinder<SampleType>::prepare (int maxBlocksize)
{
	jassert (maxBlocksize > 0);

	peakIndices.ensureStorageAllocated (maxBlocksize);
	peakSearchingOrder.ensureStorageAllocated (maxBlocksize);
	peakCandidates.ensureStorageAllocated (numPeaksToTest);
	candidateDeltas.ensureStorageAllocated (numPeaksToTest);
	finalHandful.ensureStorageAllocated (defaultFinalHandfulSize);
	finalHandfulDeltas.ensureStorageAllocated (defaultFinalHandfulSize);

	clearAllArrays (false);
}

template <typename SampleType>
void PeakFinder<SampleType>::reset()
{
	clearAllArrays (false);

	analysisFrameStart = 0;
}

template <typename SampleType>
void PeakFinder<SampleType>::releaseResources()
{
	clearAllArrays (true);

	analysisFrameStart = 0;
}

template <typename SampleType>
const Array<int>& PeakFinder<SampleType>::findPeaks (const SampleType* inputSamples, int numSamples, float period)
{
	jassert (period > 0.f && numSamples > 0);

	peakIndices.clearQuick();

	const auto grainSize  = juce::roundToInt (period * 2.f);
	const auto halfPeriod = juce::roundToInt (period * 0.5f);
	const auto intPeriod  = juce::roundToInt (period);

	jassert (numSamples >= grainSize);

	// marks the center of the analysis windows, which are 1 period long
	auto analysisIndex = analysisFrameStart + halfPeriod;

	int lastFrameRealEnd { 0 };

	do
	{
		const auto frameStart = analysisIndex - halfPeriod;

		lastFrameRealEnd = frameStart + intPeriod;

		const auto frameEnd = std::min (numSamples, lastFrameRealEnd);

		jassert (frameStart >= 0 && frameEnd <= numSamples && frameEnd > frameStart);

		peakIndices.add (findNextPeak (frameStart, frameEnd, std::min (analysisIndex, frameEnd),
									   inputSamples, intPeriod, grainSize));

		[[maybe_unused]] const auto prevAnalysisIndex = analysisIndex;

		analysisIndex = [this, intPeriod, grainSize]
		{
			const auto numPeaksFound = peakIndices.size();

			if (numPeaksFound == 1)
				return peakIndices.getUnchecked (0) + intPeriod;

			return peakIndices.getUnchecked (numPeaksFound - 2) + grainSize;
		}();

		jassert (analysisIndex > prevAnalysisIndex);
	} while (analysisIndex - halfPeriod < numSamples);

	jassert (! peakIndices.isEmpty());

	analysisFrameStart = lastFrameRealEnd - numSamples;

	jassert (analysisFrameStart >= 0);

	return peakIndices;
}

template <typename SampleType>
int PeakFinder<SampleType>::findNextPeak (int frameStart, int frameEnd, int predictedPeak,
										  const SampleType* inputSamples, int period, int grainSize)
{
	jassert (predictedPeak >= frameStart && predictedPeak <= frameEnd);

	sortSampleIndicesForPeakSearching (frameStart, frameEnd, predictedPeak);

	peakCandidates.clearQuick();

	for (auto i = 0; i < numPeaksToTest; ++i)
	{
		const auto nextPeak = getPeakCandidateInRange (inputSamples, frameStart, frameEnd, predictedPeak);

		[[unlikely]] if (nextPeak == -1) break;

		peakCandidates.add (nextPeak);
	}

	jassert (! peakCandidates.isEmpty());

	switch (peakCandidates.size())
	{
		[[unlikely]] case 1 : return peakCandidates.getUnchecked (0);

		[[unlikely]] case 2 : return choosePeakWithGreatestPower (inputSamples);

		default :
		{
			if (peakIndices.size() <= 1)
				return choosePeakWithGreatestPower (inputSamples);

			return chooseIdealPeakCandidate (inputSamples,
											 peakIndices.getLast() + period,
											 peakIndices.getUnchecked (peakIndices.size() - 2) + grainSize);
		}
	}
}


template <typename SampleType>
int PeakFinder<SampleType>::getPeakCandidateInRange (const SampleType* inputSamples,
													 int startSample, int endSample, int predictedPeak) const
{
	const auto starting = [this]
	{
		for (const auto p : peakSearchingOrder)
			if (! peakCandidates.contains (p))
				return p;

		return -1;
	}();

	[[unlikely]] if (starting == -1) return -1;

	jassert (starting >= startSample && starting <= endSample);

	const auto numSamples = endSample - startSample;

	auto get_weighted_sample = [predictedPeak, numSamples, inputSamples] (int index) -> SampleType
	{
		const auto distance = static_cast<SampleType> (std::abs (predictedPeak - index));
		const auto weight	= SampleType (1) - (distance / static_cast<SampleType> (numSamples));

		return std::abs (inputSamples[index]) * weight;
	};

	auto strongest	  = get_weighted_sample (starting);
	auto strongestIdx = starting;

	for (const auto index : peakSearchingOrder)
	{
		jassert (index >= startSample && index <= endSample);

		if (index == starting || peakCandidates.contains (index)) continue;

		const auto currentSample = get_weighted_sample (index);

		if (currentSample > strongest)
		{
			strongest	 = currentSample;
			strongestIdx = index;
		}
	}

	return strongestIdx;
}

template <typename SampleType>
int PeakFinder<SampleType>::choosePeakWithGreatestPower (const SampleType* inputSamples) const
{
	auto strongestPeakIndex = peakCandidates.getUnchecked (0);
	auto strongestPeak		= std::abs (inputSamples[strongestPeakIndex]);

	for (const auto candidate : peakCandidates)
	{
		const auto current = std::abs (inputSamples[candidate]);

		if (current > strongestPeak)
		{
			strongestPeak	   = current;
			strongestPeakIndex = candidate;
		}
	}

	return strongestPeakIndex;
}


template <typename SampleType>
int PeakFinder<SampleType>::chooseIdealPeakCandidate (const SampleType* inputSamples, int deltaTarget1, int deltaTarget2)
{
	candidateDeltas.clearQuick();
	finalHandful.clearQuick();
	finalHandfulDeltas.clearQuick();

	// 1. calculate delta values for each peak candidate
	// delta represents how far off this peak candidate is from the expected peak location - in a way it's a measure of the jitter that picking a peak candidate as this frame's peak would introduce to the overall alignment of the stream of grains based on the previous grains

	for (const auto candidate : peakCandidates)
		candidateDeltas.add ((std::abs (candidate - deltaTarget1) + std::abs (candidate - deltaTarget2)));

	// 2. whittle our remaining candidates down to the final candidates with the minimum delta values

	const auto finalHandfulSize = std::min (defaultFinalHandfulSize, candidateDeltas.size());

	for (auto i = 0; i < finalHandfulSize; ++i)
	{
		struct MinDeltaData final
		{
			int index, deltaValue;
		};

		const auto minDeltaData = [this]
		{
			MinDeltaData data;

			auto* const minIt = std::min_element (candidateDeltas.begin(), candidateDeltas.end());

			data.deltaValue = *minIt;
			data.index		= std::distance (candidateDeltas.begin(), minIt);

			return data;
		}();

		finalHandfulDeltas.add (minDeltaData.deltaValue);
		finalHandful.add (peakCandidates.getUnchecked (minDeltaData.index));

		// make sure this value won't be chosen again, w/o deleting it from the candidateDeltas array
		candidateDeltas.set (minDeltaData.index, 10000);
	}

	jassert (finalHandful.size() == finalHandfulSize && finalHandfulDeltas.size() == finalHandfulSize);

	// 3. choose the strongest overall peak from these final candidates, with peaks weighted by their delta values

	const auto deltaRange = [this]
	{
		const auto pair = std::minmax_element (finalHandfulDeltas.begin(), finalHandfulDeltas.end());

		return *pair.second - *pair.first;
	}();

	jassert (deltaRange >= 0);

	if (deltaRange == 0)  // prevent dividing by 0 in the next step...
		return finalHandful.getUnchecked (0);

	auto get_weighted_sample = [this, deltaRange, inputSamples] (int sampleIndex, int finalHandfulIdx) -> SampleType
	{
		const auto delta = static_cast<SampleType> (finalHandfulDeltas.getUnchecked (finalHandfulIdx));

		const auto deltaWeight = SampleType (1) - (delta / static_cast<SampleType> (deltaRange));

		return std::abs (inputSamples[sampleIndex]) * deltaWeight;
	};

	auto chosenPeak	   = finalHandful.getUnchecked (0);
	auto strongestPeak = get_weighted_sample (chosenPeak, 0);

	for (auto i = 1; i < finalHandfulSize; ++i)
	{
		const auto candidate = finalHandful.getUnchecked (i);

		if (candidate == chosenPeak) continue;

		const auto testingPeak = get_weighted_sample (candidate, i);

		if (testingPeak > strongestPeak)
		{
			strongestPeak = testingPeak;
			chosenPeak	  = candidate;
		}
	}

	return chosenPeak;
}

template <typename SampleType>
void PeakFinder<SampleType>::sortSampleIndicesForPeakSearching (int startSample, int endSample, int predictedPeak)
{
	jassert (predictedPeak >= startSample && predictedPeak <= endSample);
	jassert (endSample > startSample);

	peakSearchingOrder.clearQuick();

	peakSearchingOrder.set (0, predictedPeak);

	const auto searchingSize = endSample - startSample;

	for (auto p = 1, m = -1, n = 1; n < searchingSize; ++n)
	{
		const auto pos = predictedPeak + p;
		const auto neg = predictedPeak + m;

		if (math::numberIsEven (n))
		{
			if (neg >= startSample)
			{
				peakSearchingOrder.set (n, neg);
				--m;
			}
			else
			{
				jassert (pos <= endSample);
				peakSearchingOrder.set (n, pos);
				++p;
			}
		}
		else
		{
			if (pos <= endSample)
			{
				peakSearchingOrder.set (n, pos);
				++p;
			}
			else
			{
				jassert (neg >= startSample);
				peakSearchingOrder.set (n, neg);
				--m;
			}
		}
	}

	jassert (peakSearchingOrder.size() == searchingSize);
}

template class PeakFinder<float>;
template class PeakFinder<double>;

}  // namespace lemons::dsp::psola
