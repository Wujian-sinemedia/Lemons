
#pragma once

#include "Grains/SynthesisGrain.h"

namespace bav::dsp::psola
{
template < typename SampleType >
class Shifter
{
public:
    using Analyzer = Analyzer< SampleType >;
    using Grain    = psola::SynthesisGrain< SampleType >;

    Shifter (Analyzer& parentAnalyzer);

    void setPitch (float desiredFrequency, double samplerate);

    void       getSamples (SampleType* outputSamples, int numSamples);
    SampleType getNextSample();

private:
    void   startNewGrain();
    Grain* getAvailableGrain() const;
    bool   areAnyGrainsActive() const;

    Analyzer& analyzer;

    juce::OwnedArray< Grain > grains;

    int desiredPeriod {0};
    int samplesToNextGrain {0};
    int currentSample {0};  // the current sample in the frame

    events::Listener l;
};


}  // namespace bav::dsp::psola
