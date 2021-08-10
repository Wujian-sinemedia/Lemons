
#pragma once

namespace bav::dsp
{
class LFO : public osc::ChoosableOscillator< float >
{
public:
    void process (int numSamples);

private:
    void prepared (int blocksize) final;

    AudioBuffer< float > storage;
};

}  // namespace bav::dsp
