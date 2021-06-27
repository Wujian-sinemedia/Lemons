
#pragma once

#include "Grains/GrainManager.h"

namespace bav::dsp::psola
{
template < typename SampleType >
class Analyzer
{
public:
    Analyzer()  = default;
    ~Analyzer() = default;

    void prepare (double sampleRate, int blocksize);

    int getLatencySamples() const;

    void analyzeInput (const juce::AudioBuffer< SampleType >& audio);
    void analyzeInput (const SampleType* samples, int numSamples);

    int getStartOfClosestGrain (int sampleIndex) const;

    int getPeriod() const;
    int getGrainLength() const;

    events::Broadcaster& getBroadcaster();

    const AnalysisGrainStorage< SampleType >& getStorage() const;

private:
    int          getNextUnpitchedPeriod();
    juce::Random rand;

    double samplerate {0.};
    int    currentPeriod {0};

    PitchDetector< SampleType >               pitchDetector;
    psola::AnalysisGrainManager< SampleType > grains;

    events::Broadcaster broadcaster;
};


}  // namespace bav::dsp::psola
