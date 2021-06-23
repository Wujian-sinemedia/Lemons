
#pragma once


namespace bav::dsp::osc
{
template < typename SampleType, class OscType >
class OscEngine : public dsp::Engine< SampleType >
{
public:
    using AudioBuffer = juce::AudioBuffer< SampleType >;
    using MidiBuffer  = juce::MidiBuffer;

    OscEngine();

    void setFrequency (float freqHz);

private:
    void renderBlock (const AudioBuffer& input, AudioBuffer& output, MidiBuffer& midiMessages, bool isBypassed) final;
    void prepared (int blocksize, double samplerate) final;

    OscType osc;
    float   frequency {440.f};
};

}  // namespace bav::dsp::osc
