
#pragma once

#include "Grains/GrainExtractor/GrainExtractor.h"
#include "Grains/GrainStorage/GrainStorage.h"

namespace lemons::dsp::psola
{
template < typename SampleType >
class Analyzer
{
public:
    using Storage = AnalysisGrainStorage< SampleType >;

    Analyzer()  = default;
    ~Analyzer() = default;

    void prepare (double sampleRate, int blocksize);

    int getLatencySamples() const;

    void analyzeInput (const AudioBuffer< SampleType >& input, int channel = 0);
    void analyzeInput (const SampleType* samples, int numSamples);

    int getGrainLength() const;

    events::Broadcaster& getBroadcaster();
    const Storage&       getStorage() const;

    int   getPeriod() const;
    float getFrequency() const;

private:
    int          getNextUnpitchedPeriod();
    juce::Random rand;

    double samplerate {0.};
    int    currentPeriod {0};

    PitchDetector< SampleType >          pitchDetector;
    AnalysisGrainExtractor< SampleType > grainExtractor;
    Storage                              grainStorage;

    events::Broadcaster broadcaster;
};


}  // namespace bav::dsp::psola
