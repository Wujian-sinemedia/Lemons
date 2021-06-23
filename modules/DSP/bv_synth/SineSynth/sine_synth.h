
#pragma once

namespace bav::dsp
{
template < typename SampleType >
class SineSynthVoice : public SynthVoiceBase< SampleType >
{
public:
    using SynthVoiceBase< SampleType >::SynthVoiceBase;

    void renderPlease (juce::AudioBuffer< SampleType >& output, float desiredFrequency, double currentSamplerate) final;

    void released() final;
    void noteCleared() final;

private:
    osc::Sine< SampleType > sine;
};


template < typename SampleType >
struct SineSynth : public SynthBase< SampleType >
{
    using Voice = SineSynthVoice< SampleType >;

    Voice* createVoice() final;
};

using TestSynth = SineSynth< float >;


}  // namespace bav::dsp
