#pragma once

#include <lemons_dsp/lemons_dsp.h>

namespace lemons::dsp::psola
{

template<typename SampleType>
class GrainDetector final
{
public:
    const juce::Array<int>& analyzeInput (const SampleType* inputSamples,
                                          int numSamples,
                                          float period);
    
    void prepare (int maxBlocksize);
    
    void releaseResources();
    
private:
    void findPeaks (const SampleType* inputSamples, int numSamples, float period);
    
    [[nodiscard]] int findNextPeak (int frameStart, int frameEnd, int predictedPeak,
                                    const SampleType* inputSamples, float period, int grainSize);
    
    void sortSampleIndicesForPeakSearching (int startSample, int endSample, int predictedPeak);
    
    void getPeakCandidateInRange (const SampleType* inputSamples, int startSample, int endSample, int predictedPeak);
    
    [[nodiscard]] int choosePeakWithGreatestPower (const SampleType* inputSamples) const;
    
    [[nodiscard]] int chooseIdealPeakCandidate (const SampleType* inputSamples, int deltaTarget1, int deltaTarget2);
    
    juce::Array<int> grainStartIndices, peakSearchingOrder, peakCandidates, finalHandful;
    
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
struct GrainDetectorTests : public DspTest
{
    GrainDetectorTests();
    
private:
    void runTest() final;
    
    dsp::psola::GrainDetector<SampleType> grainDetector;
    
    dsp::osc::Sine<SampleType> osc;
    AudioBuffer<SampleType> audioStorage;
};

static GrainDetectorTests<float> grainDetectorTest_float;
static GrainDetectorTests<double> grainDetectorTest_double;

}

#endif
