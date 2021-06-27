
#pragma once

#include "Grains/SynthesisGrain.h"

namespace bav::dsp
{
template < typename SampleType >
class PsolaShifter
{
    using Analyzer = PsolaAnalyzer< SampleType >;
    using Grain    = psola::SynthesisGrain< SampleType >;

public:
    PsolaShifter (Analyzer& parentAnalyzer);

    void setPitch (float desiredFrequency, double samplerate);

    void       getSamples (SampleType* outputSamples, int numSamples);
    SampleType getNextSample();

private:
    void   newBlockStarting();
    void   startNewGrain();
    Grain* getAvailableGrain() const;
    bool   areAnyGrainsActive() const;

    Analyzer& analyzer;

    juce::OwnedArray< Grain > grains;

    int desiredPeriod {0};
    int samplesToNextGrain {0};
    int currentSample {0};  // the current sample in the frame

    events::Listener listener;
};


}  // namespace bav::dsp
