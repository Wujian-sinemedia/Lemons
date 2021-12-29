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

#include <lemons_core/lemons_core.h>

namespace lemons::dsp::psola
{

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

	peakIndices.clearQuick();
	peakSearchingOrder.clearQuick();
	peakCandidates.clearQuick();
	candidateDeltas.clearQuick();
	finalHandful.clearQuick();
	finalHandfulDeltas.clearQuick();
}

template <typename SampleType>
void PeakFinder<SampleType>::reset()
{
	peakIndices.clearQuick();
	peakSearchingOrder.clearQuick();
	peakCandidates.clearQuick();
	candidateDeltas.clearQuick();
	finalHandful.clearQuick();
	finalHandfulDeltas.clearQuick();

	analysisFrameStart = 0;
}

template <typename SampleType>
void PeakFinder<SampleType>::releaseResources()
{
	peakIndices.clear();
	peakSearchingOrder.clear();
	peakCandidates.clear();
	finalHandful.clear();
	candidateDeltas.clear();
	finalHandfulDeltas.clear();

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

	int lastFrameRealEnd = 0;

	do
	{
		const auto frameStart = analysisIndex - halfPeriod;

		lastFrameRealEnd = frameStart + intPeriod;

		const auto frameEnd = std::min (numSamples, lastFrameRealEnd);

		jassert (frameStart >= 0 && frameEnd <= numSamples && frameEnd > frameStart);

		peakIndices.add (findNextPeak (frameStart, frameEnd, std::min (analysisIndex, frameEnd),
		                               inputSamples, intPeriod, grainSize));

		[[maybe_unused]] const auto prevAnalysisIndex = analysisIndex;

		analysisIndex = [&]() -> int
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
	jassert (frameEnd > frameStart);
	jassert (predictedPeak >= frameStart && predictedPeak <= frameEnd);

	sortSampleIndicesForPeakSearching (frameStart, frameEnd, predictedPeak);

	jassert (peakSearchingOrder.size() == frameEnd - frameStart);

	peakCandidates.clearQuick();

	for (int i = 0; i < numPeaksToTest; ++i)
	{
		const auto nextPeak = getPeakCandidateInRange (inputSamples, frameStart, frameEnd, predictedPeak);

		if (nextPeak == -1)
			break;

		peakCandidates.add (nextPeak);
	}

	jassert (! peakCandidates.isEmpty());

	switch (peakCandidates.size())
	{
		case 1 : return peakCandidates.getUnchecked (0);

		case 2 : return choosePeakWithGreatestPower (inputSamples);

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
	jassert (! peakSearchingOrder.isEmpty());

	const auto starting = [&]() -> int
	{
		for (const auto p : peakSearchingOrder)
			if (! peakCandidates.contains (p))
				return p;

		return -1;
	}();

	if (starting == -1)
		return -1;

	jassert (starting >= startSample && starting <= endSample);

	const auto numSamples = endSample - startSample;

	const auto get_weighted_sample = [&] (int index) -> SampleType
	{
		const auto distance = static_cast<double> (std::abs (predictedPeak - index));
		const auto weight   = 1. - (distance / static_cast<double> (numSamples));

		return inputSamples[index] * static_cast<SampleType> (weight);
	};

	auto localMin        = get_weighted_sample (starting);
	auto localMax        = localMin;
	auto indexOfLocalMin = starting;
	auto indexOfLocalMax = starting;

	for (const auto index : peakSearchingOrder)
	{
		jassert (index >= startSample && index <= endSample);

		if (index == starting || peakCandidates.contains (index)) continue;

		const auto currentSample = get_weighted_sample (index);

		if (currentSample < localMin)
		{
			localMin        = currentSample;
			indexOfLocalMin = index;
		}

		if (currentSample > localMax)
		{
			localMax        = currentSample;
			indexOfLocalMax = index;
		}
	}

	if (std::abs (localMin) > std::abs (localMax))
		return indexOfLocalMin;

	return indexOfLocalMax;
}

template <typename SampleType>
int PeakFinder<SampleType>::choosePeakWithGreatestPower (const SampleType* inputSamples) const
{
	auto strongestPeakIndex = peakCandidates.getUnchecked (0);
	auto strongestPeak      = std::abs (inputSamples[strongestPeakIndex]);

	for (const auto candidate : peakCandidates)
	{
		const auto current = std::abs (inputSamples[candidate]);

		if (current > strongestPeak)
		{
			strongestPeak      = current;
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

	float minimum      = 0.f;
	int   minimumIndex = 0;

	for (int i = 0; i < finalHandfulSize; ++i)
	{
		findMinDelta (minimum, minimumIndex);

		finalHandfulDeltas.add (minimum);
		finalHandful.add (peakCandidates.getUnchecked (minimumIndex));

		// make sure this value won't be chosen again, w/o deleting it from the candidateDeltas array
		candidateDeltas.set (minimumIndex, 10000.f);
	}

	jassert (finalHandful.size() == finalHandfulSize && finalHandfulDeltas.size() == finalHandfulSize);

	// 3. choose the strongest overall peak from these final candidates, with peaks weighted by their delta values

	const auto deltaRange = juce::FloatVectorOperations::findMinAndMax (finalHandfulDeltas.getRawDataPointer(),
	                                                                    finalHandfulDeltas.size())
	                            .getLength();

	if (deltaRange < 0.05f)  // prevent dividing by 0 in the next step...
		return finalHandful.getUnchecked (0);

	const auto get_weighted_sample = [&] (int sampleIndex, int finalHandfulIdx) -> SampleType
	{
		const auto delta = finalHandfulDeltas.getUnchecked (finalHandfulIdx);

		const auto deltaWeight = 1.f - (delta / static_cast<float> (deltaRange));

		return std::abs (inputSamples[sampleIndex]) * static_cast<SampleType> (deltaWeight);
	};

	auto chosenPeak    = finalHandful.getUnchecked (0);
	auto strongestPeak = get_weighted_sample (chosenPeak, 0);

	for (int i = 1; i < finalHandfulSize; ++i)
	{
		const auto candidate = finalHandful.getUnchecked (i);

		if (candidate == chosenPeak) continue;

		const auto testingPeak = get_weighted_sample (candidate, i);

		if (testingPeak > strongestPeak)
		{
			strongestPeak = testingPeak;
			chosenPeak    = candidate;
		}
	}

	return chosenPeak;
}

template <typename SampleType>
void PeakFinder<SampleType>::findMinDelta (float& minDelta, int& index) const
{
	minDelta = candidateDeltas.getUnchecked (0);

	for (int i = 0; i < candidateDeltas.size(); ++i)
	{
		const auto current = candidateDeltas.getUnchecked (i);

		if (current < minDelta)
		{
			minDelta = current;
			index    = i;
		}
	}
}

template <typename SampleType>
void PeakFinder<SampleType>::sortSampleIndicesForPeakSearching (int startSample, int endSample, int predictedPeak)
{
	peakSearchingOrder.clearQuick();

	jassert (predictedPeak >= startSample && predictedPeak <= endSample);

	peakSearchingOrder.set (0, predictedPeak);

	for (int p = 1, m = -1, n = 1;
	     n < (endSample - startSample);
	     ++n)
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
}

template class PeakFinder<float>;
template class PeakFinder<double>;

}  // namespace lemons::dsp::psola


/*---------------------------------------------------------------------------------------------------------------------------------*/

#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

template <typename SampleType>
PeakFinderTests<SampleType>::PeakFinderTests()
    : DspTest (getDspTestName<SampleType> ("PSOLA grain detector tests"))
{
}

template <typename SampleType>
void PeakFinderTests<SampleType>::runTest()
{
	constexpr auto samplerate = 44100.;

	for (const auto freq : { 85., 215., 440., 755., 2036., 3531. })
	{
		const auto period = math::periodInSamples (samplerate, freq);

		beginTest ("Period: " + String (period) + " samples");

		const auto blocksize = period * 4;

		peakFinder.prepare (blocksize);

		audioStorage.setSize (1, blocksize);

		runOscillatorTest (sine, samplerate, freq, blocksize, period, "Sine");
		runOscillatorTest (saw, samplerate, freq, blocksize, period, "Saw");
		runOscillatorTest (square, samplerate, freq, blocksize, period, "Square");
		runOscillatorTest (triangle, samplerate, freq, blocksize, period, "Triangle");
	}

	{
		const auto subtest = beginSubtest ("Random noise");

		fillAudioBufferWithRandomNoise (audioStorage, getRandom());

		const auto numSamples = audioStorage.getNumSamples();

		runBufferTest (numSamples, nextRandomPeriod (samplerate, 60, numSamples));
	}

	{
		const auto subtest = beginSubtest ("Silence");

		audioStorage.clear();

		const auto numSamples = audioStorage.getNumSamples();

		runBufferTest (numSamples, nextRandomPeriod (samplerate, 60, numSamples));
	}
}

template <typename SampleType>
void PeakFinderTests<SampleType>::runOscillatorTest (dsp::osc::Oscillator<SampleType>& osc,
                                                     double                            samplerate,
                                                     double                            freq,
                                                     int                               blocksize,
                                                     int                               period,
                                                     const String&                     waveName)
{
	const auto oscSubtest = beginSubtest (waveName + " oscillator");

	osc.setFrequency (static_cast<SampleType> (freq),
	                  static_cast<SampleType> (samplerate));

	osc.getSamples (audioStorage);

	runBufferTest (blocksize, period);
}

template <typename SampleType>
void PeakFinderTests<SampleType>::runBufferTest (int blocksize, int period)
{
	const auto& indices = peakFinder.findPeaks (audioStorage.getReadPointer (0), blocksize, period);

	{
		const auto subtest = beginSubtest ("Grains identified");
		expectGreaterOrEqual (indices.size(), (blocksize / period) - 1);
	}

	{
		const auto subtest = beginSubtest ("Indices are always increasing");

		for (int i = 0; i < indices.size() - 1; ++i)
			expectGreaterThan (indices.getUnchecked (i + 1),
			                   indices.getUnchecked (i));
	}

	{
		const auto subtest = beginSubtest ("Max and min index are within range");

		expectGreaterOrEqual (indices.getUnchecked (0), 0);

		expectLessOrEqual (indices.getUnchecked (indices.size() - 1), blocksize);
	}

	const auto subtest = beginSubtest ("Grain spacing");

	/*
	 Heuristics:
	 - grains are approx 2 periods long
	 - grains overlap by approx 50%
	 - grains are centered on points of max energy in signal
	 - individual grain onsets are approx 1 period apart
	 */

	const auto halfPeriod = period / 2;

	for (int i = 0; i < indices.size() - 2; ++i)
	{
		const auto index1 = indices.getUnchecked (i);
		const auto index2 = indices.getUnchecked (i + 1);
		const auto index3 = indices.getUnchecked (i + 2);

		expectWithinAbsoluteError (index3 - index1, period * 2, halfPeriod);

		expectWithinAbsoluteError (index2 - index1, period, halfPeriod);
		expectWithinAbsoluteError (index3 - index2, period, halfPeriod);
	}
}

template <typename SampleType>
int PeakFinderTests<SampleType>::nextRandomPeriod (double samplerate, int minHz, int numSamples)
{
	const auto maxPeriod = std::min (math::periodInSamples (samplerate, minHz), numSamples / 2);
	const auto minPeriod = std::min (numSamples / 4, maxPeriod - 1);

	return getRandom().nextInt ({ minPeriod, maxPeriod + 1 });
}

template struct PeakFinderTests<float>;
template struct PeakFinderTests<double>;

}  // namespace lemons::tests

#endif
