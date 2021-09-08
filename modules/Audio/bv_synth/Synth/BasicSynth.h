
#pragma once

#include <bv_oscillators/bv_oscillators.h>

namespace bav::dsp
{
template < typename SampleType, template < typename NumericType > class OscType >
struct BasicSynthVoice : public SynthVoiceBase< SampleType >
{
    using SynthVoiceBase< SampleType >::SynthVoiceBase;

    void renderPlease (AudioBuffer< SampleType >& output, float desiredFrequency, double currentSamplerate) final;

    void released() final;
    void noteCleared() final;

    OscType< SampleType > osc;
};

template < typename SampleType, template < typename NumericType > class OscType >
struct BasicSynth : public SynthBase< SampleType >
{
    SynthVoiceBase< SampleType >* createVoice() final;
};

template < typename SampleType >
using SineSynth = BasicSynth< SampleType, osc::Sine >;

}  // namespace bav::dsp
