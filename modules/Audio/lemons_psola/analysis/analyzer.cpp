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
	jassert (numSamples >= getLatencySamples());

	newBlockStarting();

	if (const auto detectedPeriod = pitchDetector.detectPeriod (inputAudio, numSamples);
	    detectedPeriod > 0.f)
		currentPeriod = detectedPeriod;
	else
		currentPeriod = numSamples / 4;  // if frame is unpitched, assign a constant period value to use...

	jassert (currentPeriod > 0.f);

	makeWindow();

	const auto intPeriod = juce::roundToInt (currentPeriod);
	const auto grainSize = intPeriod * 2;

	for (const auto peak : peakFinder.findPeaks (inputAudio, numSamples, currentPeriod))
	{
		const auto start = peak - intPeriod;
		const auto end   = peak + intPeriod;

		if (start < 0 || end >= numSamples)  // TO DO
			continue;

		jassert (end - start == grainSize);

		auto& grain = getGrainToStoreIn();

		grain.origStartIndex = start;
		grain.grainSize      = grainSize;

		grain.samples.setSize (1, grainSize, true, true, true);

		using FVO = juce::FloatVectorOperations;

		auto* const destSamples = grain.samples.getWritePointer (0);

		FVO::copy (destSamples, inputAudio + start, grainSize);

		FVO::multiply (destSamples, window.getRawDataPointer(), grainSize);
	}

	lastBlocksize = numSamples;
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
void Analyzer<SampleType>::newBlockStarting()
{
	// move up previous grains
	if (lastBlocksize > 0)
		for (auto* grain : grains)
			grain->origStartIndex -= lastBlocksize;

	for (auto* shifter : shifters)
		shifter->newBlockStarting();
}

inline double ncos (int order, int i, int size) noexcept
{
	return std::cos (static_cast<double> (order * i)
	                 * juce::MathConstants<double>::pi / static_cast<double> (size - 1));
}

template <typename SampleType>
void Analyzer<SampleType>::makeWindow()
{
	window.clearQuick();

	const auto size = juce::roundToInt (currentPeriod) * 2;

	for (int i = 0; i < size; ++i)
	{
		const auto cos2 = ncos (2, i, size);

		window.set (i, static_cast<SampleType> (0.5 - 0.5 * cos2));
	}
}

template <typename SampleType>
typename Analyzer<SampleType>::Grain& Analyzer<SampleType>::getClosestGrain (int placeInBlock)
{
	jassert (! grains.isEmpty());

	Grain* closest { nullptr };
	int    distance { std::numeric_limits<int>::max() };

	for (auto* grain : grains)
	{
		if (grain->origStartIndex < 0)
			continue;

		const auto currentDist = std::abs (grain->origStartIndex - placeInBlock);

		if (currentDist < distance)
		{
			distance = currentDist;
			closest  = grain;
		}
	}

	jassert (closest != nullptr);

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

	pitchDetector.setSamplerate (newSamplerate);

	for (auto* shifter : shifters)
		if (shifter->targetPitchHz > 0)
			shifter->setPitch (shifter->targetPitchHz, newSamplerate);

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

	//    while (grains.size() < latency / 2)
	//        grains.add (new Grain);
	//
	//    for (auto* grain : grains)
	//        grain->samples.setSize (1, latency, true, true, true);

	return latency;
}

template <typename SampleType>
void Analyzer<SampleType>::releaseResources()
{
	for (auto* shifter : shifters)
		shifter->releaseResources();

	peakFinder.releaseResources();

	lastBlocksize = 0;
	currentPeriod = 0.f;

	grains.clearQuick (true);
	window.clearQuick();
}

template class Analyzer<float>;
template class Analyzer<double>;

}  // namespace lemons::dsp::psola
