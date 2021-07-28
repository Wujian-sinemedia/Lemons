
#pragma once

namespace bav::dsp
{
template < typename SampleType >
class LFO : public osc::ChoosableOscillator< SampleType >
{
public:
    void process (int numSamples);

private:
    void prepared (int blocksize) final;

    AudioBuffer< SampleType > storage;
};

}  // namespace bav::dsp
