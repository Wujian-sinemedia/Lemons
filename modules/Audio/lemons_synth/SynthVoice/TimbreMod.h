#pragma once

#include <lemons_audio_effects/lemons_audio_effects.h>


namespace lemons::dsp::synth
{
template < typename SampleType >
class TimbreMod
{
public:
    TimbreMod (const FX::FilterParams& filterParamsToUse, const float& gainValueToUse, const bool& filterToggleToUse);
    TimbreMod (const typename SynthBase< SampleType >::TimbreModParams& modSource);

    void prepare (int blocksize, double samplerate);

    void process (AudioBuffer< SampleType >& audio);

    void skipSamples (int numSamples);

    void reset();

    void setToggle (bool shouldModBeOn);

private:
    const FX::FilterParams& filterParams;

    const float& gainValue;

    const bool& filterToggle;

    FX::FilterEngine< SampleType > filter {filterParams};

    FX::SmoothedGain< SampleType, 1 > gain;

    bool toggle {false};
};

}  // namespace lemons::dsp::synth
