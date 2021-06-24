
#pragma once

namespace bav::dsp::FX
{
template < typename SampleType >
class StereoPanner : public PannerBase, public AudioEffect< SampleType >
{
public:
    using AudioBuffer = juce::AudioBuffer< SampleType >;

    void prepare (double samplerate, int blocksize);
    void reset();

    void process (const AudioBuffer& stereoInput,
                  AudioBuffer&       stereoOutput);

    void process (AudioBuffer& audio) final;

private:
    SmoothedGain< SampleType, 1 > left, right;
};

}  // namespace bav::dsp::FX
