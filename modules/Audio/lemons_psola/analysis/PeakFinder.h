#pragma once

#include <lemons_dsp/lemons_dsp.h>

namespace lemons::dsp::psola
{

template<typename SampleType>
class PeakFinder final
{
public:
    const juce::Array<int>& findPeaks (const SampleType* inputSamples, int numSamples, float period);
    
    void prepare (int maxBlocksize);
    
    void releaseResources();
    
private:
    [[nodiscard]] int findNextPeak (int frameStart, int frameEnd, int predictedPeak,
                                    const SampleType* inputSamples, float period, int grainSize);
    
    void sortSampleIndicesForPeakSearching (int startSample, int endSample, int predictedPeak);
    
    void getPeakCandidateInRange (const SampleType* inputSamples, int startSample, int endSample, int predictedPeak);
    
    [[nodiscard]] int choosePeakWithGreatestPower (const SampleType* inputSamples) const;
    
    [[nodiscard]] int chooseIdealPeakCandidate (const SampleType* inputSamples, int deltaTarget1, int deltaTarget2);
    
    juce::Array<int>   peakIndices, peakSearchingOrder, peakCandidates, finalHandful;
    juce::Array<float> candidateDeltas, finalHandfulDeltas;
    
    static constexpr auto numPeaksToTest = 5;
    static constexpr auto defaultFinalHandfulSize = 3;
};

}


/*---------------------------------------------------------------------------------------------------------------------------------*/

#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

template<typename SampleType>
struct PeakFinderTests : public DspTest
{
    PeakFinderTests();
    
private:
    void runTest() final;
    
    void runOscillatorTest (dsp::osc::Oscillator<SampleType>& osc, double samplerate, double freq, int blocksize, int period);
    
    dsp::psola::PeakFinder<SampleType> peakFinder;
    
    dsp::osc::Sine<SampleType> osc;
    AudioBuffer<SampleType> audioStorage;
};

static PeakFinderTests<float> grainDetectorTest_float;
static PeakFinderTests<double> grainDetectorTest_double;

}

#endif
