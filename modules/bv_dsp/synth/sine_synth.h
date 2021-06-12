
#pragma once

namespace bav::dsp
{
template < typename SampleType >
class SineSynthVoice : public SynthVoiceBase< SampleType >
{
public:
    SineSynthVoice (SynthBase< SampleType >* base)
        : SynthVoiceBase< SampleType > (base)
    {
    }

    void renderPlease (juce::AudioBuffer< SampleType >& output, float desiredFrequency, double currentSamplerate) final
    {
        sine.setFrequency (SampleType (desiredFrequency), SampleType (currentSamplerate));
        sine.getSamples (output.getWritePointer (0), output.getNumSamples());
    }

    void released() final { sine.resetPhase(); }
    void noteCleared() final { sine.resetPhase(); }

private:
    osc::Sine< SampleType > sine;
};


template class SineSynthVoice< float >;
template class SineSynthVoice< double >;


template < typename SampleType >
struct SineSynth : public SynthBase< SampleType >
{
    using Voice = SineSynthVoice< SampleType >;

    Voice* createVoice() final
    {
        return new Voice (this);
    }
};

template struct SineSynth< float >;
template struct SineSynth< double >;


using TestSynth = SineSynth< float >;


}  // namespace bav::dsp
