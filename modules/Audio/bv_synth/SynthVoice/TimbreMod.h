#pragma once

namespace bav::dsp::synth
{
template < typename SampleType >
class TimbreMod
{
public:
    TimbreMod (FX::FilterParams& filterParamsToUse);

    void prepare (int blocksize, double samplerate);

    void process (AudioBuffer< SampleType >& audio);

    void skipSamples (int numSamples);

    void reset();

    void setGain (float newGain);
    void setToggle (bool shouldModBeOn);

private:
    FX::FilterParams& filterParams;

    FX::FilterEngine< SampleType > filter {filterParams};

    FX::SmoothedGain< SampleType, 1 > gain;

    bool toggle;
};

}  // namespace bav::dsp::synth
