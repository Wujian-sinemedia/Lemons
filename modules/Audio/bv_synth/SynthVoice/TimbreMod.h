#pragma once

namespace bav::dsp::synth
{
template < typename SampleType >
class TimbreMod
{
public:
    TimbreMod (FX::FilterParams& filterParamsToUse, const float& gainValueToUse);

    void prepare (int blocksize, double samplerate);

    void process (AudioBuffer< SampleType >& audio);

    void skipSamples (int numSamples);

    void reset();

    void setToggle (bool shouldModBeOn);
    void setFilterToggle (bool shouldUseFilter);

private:
    FX::FilterParams& filterParams;

    const float& gainValue;

    FX::FilterEngine< SampleType > filter {filterParams};

    FX::SmoothedGain< SampleType, 1 > gain;

    bool filterToggle {true}, toggle {false};
};

}  // namespace bav::dsp::synth
