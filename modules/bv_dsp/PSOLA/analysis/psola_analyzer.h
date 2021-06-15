
#pragma once

#include "GrainExtractor/GrainExtractor.h"


namespace bav::dsp
{
template < typename SampleType >
class PsolaAnalyzer
{
public:
    PsolaAnalyzer();
    ~PsolaAnalyzer() = default;
    
    void analyzeInput (const juce::AudioBuffer<SampleType>& audio);
    void analyzeInput (const SampleType* samples, int numSamples);
    
    int getPeriod() const { return currentPeriod; }
    
    int getClosestGrainOnset (int sampleIndex) const;

private:
    int getNextUnpitchedPeriod();
    juce::Random rand;
    
    double samplerate {0.};
    int    currentPeriod {0};

    PitchDetector< SampleType >  pitchDetector;
    GrainExtractor< SampleType > grainExtractor;
    juce::Array<int>             grainOnsetIndices;
};


}  // namespace bav::dsp
