
#pragma once

#include "Grains/RespacedGrain.h"

namespace bav::dsp
{
template < typename SampleType >
class PsolaShifter
{
    using Analyzer = PsolaAnalyzer< SampleType >;
    using Grain    = psola::RespacedGrain< SampleType >;

public:
    PsolaShifter (Analyzer& parentAnalyzer);

    void setPitch (float desiredFrequency, double samplerate);

    void       getSamples (SampleType* outputSamples, int numSamples);
    SampleType getNextSample();

private:
    void newBlockStarting();

    bool areAnyGrainsActive() const;
    void startNewGrain();

    Grain* getAvailableGrain() const;

    Analyzer& analyzer;

    int desiredPeriod {0};

    juce::OwnedArray< Grain > grains;

    int currentSample {0};

    events::Listener listener;
};


}  // namespace bav::dsp
