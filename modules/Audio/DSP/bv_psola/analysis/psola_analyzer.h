
#pragma once

#include "Grains/GrainManager.h"


namespace bav::dsp
{
template < typename SampleType >
class PsolaAnalyzer
{
public:
    PsolaAnalyzer()  = default;
    ~PsolaAnalyzer() = default;

    void prepare (double sampleRate, int blocksize);
    
    int getLatencySamples() const;

    void analyzeInput (const juce::AudioBuffer< SampleType >& audio);
    void analyzeInput (const SampleType* samples, int numSamples);

    int getPeriod() const;

private:
    int          getNextUnpitchedPeriod();
    juce::Random rand;

    double samplerate {0.};
    int    currentPeriod {0};

    PitchDetector< SampleType >               pitchDetector;
    psola::AnalysisGrainManager< SampleType > grains;
};


}  // namespace bav::dsp
