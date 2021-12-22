#pragma once

#include <juce_core/juce_core.h>

namespace lemons::dsp::psola
{

template <typename T>
using Array = juce::Array<T>;

template <typename SampleType>
class PeakFinder final
{
public:
	[[nodiscard]] const Array<int>& findPeaks (const SampleType* inputSamples, int numSamples, float period);

	void prepare (int maxBlocksize);

	void releaseResources();

private:
	[[nodiscard]] int findNextPeak (int frameStart, int frameEnd, int predictedPeak,
	                                const SampleType* inputSamples, int period, int grainSize);

	void sortSampleIndicesForPeakSearching (int startSample, int endSample, int predictedPeak);

	[[nodiscard]] int getPeakCandidateInRange (const SampleType* inputSamples, int startSample, int endSample, int predictedPeak) const;

	[[nodiscard]] int choosePeakWithGreatestPower (const SampleType* inputSamples) const;

	[[nodiscard]] int chooseIdealPeakCandidate (const SampleType* inputSamples, int deltaTarget1, int deltaTarget2);

	void findMinDelta (float& minDelta, int& index) const;

	Array<int>   peakIndices, peakSearchingOrder, peakCandidates, finalHandful;
	Array<float> candidateDeltas, finalHandfulDeltas;

	static constexpr auto numPeaksToTest          = 5;
	static constexpr auto defaultFinalHandfulSize = 3;
};

}  // namespace lemons::dsp::psola


/*---------------------------------------------------------------------------------------------------------------------------------*/

#if LEMONS_UNIT_TESTS

#  include <lemons_dsp/lemons_dsp.h>

namespace lemons::tests
{

template <typename SampleType>
struct PeakFinderTests : public DspTest
{
	PeakFinderTests();

private:
	void runTest() final;

	void runOscillatorTest (dsp::osc::Oscillator<SampleType>& osc, double samplerate, double freq, int blocksize, int period, const String& waveName);

	dsp::psola::PeakFinder<SampleType> peakFinder;

	AudioBuffer<SampleType> audioStorage;

	dsp::osc::Sine<SampleType>     sine;
	dsp::osc::Saw<SampleType>      saw;
	dsp::osc::Square<SampleType>   square;
	dsp::osc::Triangle<SampleType> triangle;
};

LEMONS_CREATE_DSP_TEST (PeakFinderTests)

}  // namespace lemons::tests

#endif
