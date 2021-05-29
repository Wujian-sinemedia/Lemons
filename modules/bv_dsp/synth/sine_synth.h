
#pragma once

namespace bav::dsp
{
template < typename SampleType >
class SineSynthVoice : public SynthVoiceBase< SampleType >
{
    using Base        = bav::dsp::SynthVoiceBase< SampleType >;
    using Parent      = bav::dsp::SynthBase< SampleType >;
    using AudioBuffer = juce::AudioBuffer< SampleType >;

public:
    SineSynthVoice (Parent* base, double initSamplerate = 44100.0)
        : Base (base, initSamplerate)
    {
    }

    /*
            This function will be called by the SynthVoiceBase class when it's time to generate some more audio. This function may ask for as little as 1 sample at a time.
            The most up-to-date information about what pitch this voice should be generating, based on note events, pitch bend, microtunings, and everything else the base classes manage for you, will ALWAYS be the desiredFrequency argument provided here.
            The currentSamplerate argument is provided for convenience.
        */
    void renderPlease (AudioBuffer& output, float desiredFrequency, double currentSamplerate, int) override
    {
        sine.setFrequency (SampleType (desiredFrequency), SampleType (currentSamplerate));
        sine.getSamples (output.getWritePointer (0), output.getNumSamples());
    }

    void released() override { sine.resetPhase(); }

    void noteCleared() override { sine.resetPhase(); }


private:
    bav::dsp::osc::Sine< SampleType > sine;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SineSynthVoice)
};


template class SineSynthVoice< float >;
template class SineSynthVoice< double >;


template < typename SampleType >
class SineSynth : public SynthBase< SampleType >
{
    using Base  = bav::dsp::SynthBase< SampleType >;
    using Voice = SineSynthVoice< SampleType >;

public:
    SineSynth() = default;

    Voice* createVoice()
    {
        return new Voice (this);
    }


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SineSynth)
};


template class SineSynth< float >;
template class SineSynth< double >;


}  // namespace bav::dsp
