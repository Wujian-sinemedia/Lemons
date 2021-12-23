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
Analyzer<SampleType>::Analyzer (int minFreqHz)
{
	setMinInputFreq (minFreqHz);
}

template <typename SampleType>
void Analyzer<SampleType>::registerShifter (Shifter<SampleType>& shifter)
{
	jassert (! shifters.contains (&shifter));

	shifters.add (&shifter);
}

template <typename SampleType>
void Analyzer<SampleType>::deregisterShifter (Shifter<SampleType>& shifter)
{
	shifters.removeFirstMatchingValue (&shifter);
}

template <typename SampleType>
void Analyzer<SampleType>::analyzeInput (const AudioBuffer<SampleType>& inputAudio)
{
	analyzeInput (inputAudio.getReadPointer (0), inputAudio.getNumSamples());
}

template <typename SampleType>
void Analyzer<SampleType>::analyzeInput (const SampleType* inputAudio, int numSamples)
{
	jassert (samplerate > 0.);
	jassert (numSamples >= getLatencySamples());

	for (auto* grain : grains)
		grain->newBlockStarting (lastBlocksize);

	for (auto* shifter : shifters)
		shifter->newBlockStarting();

	currentPeriod = [&]() -> float
	{
		const auto detectedPeriod = pitchDetector.detectPeriod (inputAudio, numSamples);

		if (detectedPeriod > 0.f)
			return detectedPeriod;

		return static_cast<float> (random.nextInt ({ pitchDetector.getMinHz(), numSamples / 2 }));
	}();

	jassert (currentPeriod > 0.f && currentPeriod <= numSamples / 2);

	if (! incompleteGrainsFromLastFrame.isEmpty())
		makeWindow (lastFrameGrainSize);

	for (const auto grainStartInLastFrame : incompleteGrainsFromLastFrame)
	{
		jassert (lastFrameGrainSize > 0);

		const auto samplesFromLastFrame = lastBlocksize - grainStartInLastFrame;

		getGrainToStoreIn().storeNewGrain (prevFrame.getReadPointer (0), grainStartInLastFrame, samplesFromLastFrame,
		                                   inputAudio, lastFrameGrainSize - samplesFromLastFrame,
		                                   window.getRawDataPointer(), lastFrameGrainSize, -grainStartInLastFrame);
	}

	incompleteGrainsFromLastFrame.clearQuick();

	const auto grainSize = juce::roundToInt (currentPeriod * 2.f);

	makeWindow (grainSize);

	for (const auto peak : peakFinder.findPeaks (inputAudio, numSamples, currentPeriod))
	{
		const auto start = juce::roundToInt (static_cast<float> (peak) - currentPeriod);
		const auto end   = juce::roundToInt (static_cast<float> (peak) + currentPeriod);

		if (start < 0)
		{
			if (lastBlocksize == 0)
				continue;

			const auto samplesFromPrevFrame = grainSize + start;
			jassert (samplesFromPrevFrame > 0);

			getGrainToStoreIn().storeNewGrain (prevFrame.getReadPointer (0), lastBlocksize - samplesFromPrevFrame, samplesFromPrevFrame,
			                                   inputAudio, grainSize - samplesFromPrevFrame,
			                                   window.getRawDataPointer(), grainSize, start);

			continue;
		}

		if (end >= numSamples)
		{
			incompleteGrainsFromLastFrame.add (start);
			continue;
		}

		jassert (end - start == grainSize);

		getGrainToStoreIn().storeNewGrain (inputAudio, start, window.getRawDataPointer(), grainSize);
	}

	juce::FloatVectorOperations::copy (prevFrame.getWritePointer (0), inputAudio, numSamples);

	lastBlocksize      = numSamples;
	lastFrameGrainSize = grainSize;
}

template <typename SampleType>
typename Analyzer<SampleType>::Grain& Analyzer<SampleType>::getGrainToStoreIn()
{
	for (auto* grain : grains)
		if (grain->getReferenceCount() == 0)
			return *grain;

	return *grains.add (new Grain);
}

template <typename SampleType>
inline void Analyzer<SampleType>::makeWindow (int size)
{
	window.clearQuick();

	// Hanning window function
	for (int i = 0; i < size; ++i)
	{
		const auto cos2 = std::cos (static_cast<double> (2 * i)
		                            * juce::MathConstants<double>::pi / static_cast<double> (size - 1));

		window.set (i, static_cast<SampleType> (0.5 - 0.5 * cos2));
	}
}

template <typename SampleType>
typename Analyzer<SampleType>::Grain& Analyzer<SampleType>::getClosestGrain (int placeInBlock) const
{
	jassert (! grains.isEmpty());
	jassert (placeInBlock >= 0);

	Grain* closest { nullptr };
	auto   distance { std::numeric_limits<int>::max() };

	for (auto* grain : grains)
	{
		if (grain->getSize() == 0)
			continue;
        
		const auto currentDist = placeInBlock - grain->getOrigStart();
        
		if (currentDist < distance)
		{
			distance = currentDist;
			closest  = grain;
		}
	}

	jassert (closest != nullptr);
	jassert (closest->getSize() > 0);

	return *closest;
}

template <typename SampleType>
[[nodiscard]] int Analyzer<SampleType>::getLatencySamples() const noexcept
{
	return pitchDetector.getLatencySamples();
}

template <typename SampleType>
int Analyzer<SampleType>::setSamplerate (double newSamplerate)
{
	jassert (newSamplerate > 0.);

	samplerate = newSamplerate;

	pitchDetector.setSamplerate (newSamplerate);

	for (auto* shifter : shifters)
		shifter->samplerateChanged();

	return latencyChanged();
}

template <typename SampleType>
int Analyzer<SampleType>::setMinInputFreq (int minFreqHz)
{
	jassert (minFreqHz > 0);

	pitchDetector.setMinHz (minFreqHz);

	return latencyChanged();
}

template <typename SampleType>
inline int Analyzer<SampleType>::latencyChanged()
{
	const auto latency = getLatencySamples();

	peakFinder.prepare (latency);
	window.ensureStorageAllocated (latency);
	incompleteGrainsFromLastFrame.ensureStorageAllocated (latency / 2);

	prevFrame.setSize (1, latency, true, true, true);

	while (grains.size() < latency / 2)
		grains.add (new Grain);

	for (auto* grain : grains)
		grain->reserveSize (latency);

	for (auto* shifter : shifters)
		shifter->latencyChanged();

	return latency;
}

template <typename SampleType>
void Analyzer<SampleType>::releaseResources()
{
	for (auto* shifter : shifters)
		shifter->releaseResources();

	peakFinder.releaseResources();

	samplerate         = 0.;
	lastBlocksize      = 0;
	currentPeriod      = 0.f;
	lastFrameGrainSize = 0;

	prevFrame.setSize (0, 0);

	grains.clearQuick (true);
	window.clearQuick();
	incompleteGrainsFromLastFrame.clearQuick();
}

/*---------------------------------------------------------------------------------------------------------------------------*/

template <typename SampleType>
SampleType Analyzer<SampleType>::Grain::getSample (int index) const noexcept
{
	jassert (index >= 0 && index < grainSize);

	return samples.getSample (0, index);
}

template <typename SampleType>
void Analyzer<SampleType>::Grain::storeNewGrain (const SampleType* origSamples, int startIndex,
                                                 const SampleType* windowSamples, int numSamples)
{
	storeNewGrain (origSamples, startIndex, numSamples, nullptr, 0, windowSamples, numSamples, startIndex);
}

template <typename SampleType>
void Analyzer<SampleType>::Grain::storeNewGrain (const SampleType* origSamples1, int startIndex1, int blocksize1,
                                                 const SampleType* origSamples2, int blocksize2,
                                                 const SampleType* windowSamples, int totalNumSamples,
                                                 int grainStartIdx)
{
	jassert (getReferenceCount() == 0);
	jassert (totalNumSamples > 0);
	jassert (totalNumSamples == blocksize1 + blocksize2);

	origStartIndex = grainStartIdx;
	grainSize      = totalNumSamples;

	samples.setSize (1, totalNumSamples, true, true, true);

	auto* const destSamples = samples.getWritePointer (0);

	using FVO = juce::FloatVectorOperations;

	jassert (blocksize1 > 0);
	jassert (startIndex1 >= 0);

	FVO::copy (destSamples, origSamples1 + startIndex1, blocksize1);

	if (blocksize2 > 0)
		FVO::copy (destSamples + startIndex1, origSamples2, blocksize2);

	FVO::multiply (destSamples, windowSamples, grainSize);
}

template <typename SampleType>
void Analyzer<SampleType>::Grain::newBlockStarting (int last_blocksize)
{
	if (getReferenceCount() > 0)
		origStartIndex -= last_blocksize;
	else
		grainSize = 0;
}

template <typename SampleType>
int Analyzer<SampleType>::Grain::getSize() const noexcept
{
	return grainSize;
}

template <typename SampleType>
int Analyzer<SampleType>::Grain::getOrigStart() const noexcept
{
	return origStartIndex;
}

template <typename SampleType>
void Analyzer<SampleType>::Grain::reserveSize (int numSamples)
{
	samples.setSize (1, numSamples, true, true, true);
}

template class Analyzer<float>;
template class Analyzer<double>;

}  // namespace lemons::dsp::psola
