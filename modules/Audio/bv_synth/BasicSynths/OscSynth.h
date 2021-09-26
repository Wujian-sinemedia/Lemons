
#pragma once

#include <bv_oscillators/bv_oscillators.h>

namespace bav::dsp
{
template < typename SampleType, template < typename NumericType > class OscType >
struct OscSynthVoice : public SynthVoiceBase< SampleType >
{
    using SynthVoiceBase< SampleType >::SynthVoiceBase;

private:
    void renderPlease (AudioBuffer< SampleType >& output, float desiredFrequency, double currentSamplerate) final;

    void released() final;
    void noteCleared() final;

    OscType< SampleType > osc;
};


template < typename SampleType, template < typename NumericType > class OscType >
struct OscSynth : public TemplateSynth< SampleType, OscSynthVoice< SampleType, OscType > >
{
};


namespace synth
{
template < typename SampleType >
using Sine = OscSynth< SampleType, osc::Sine >;

template < typename SampleType >
using Saw = OscSynth< SampleType, osc::Saw >;

template < typename SampleType >
using Square = OscSynth< SampleType, osc::Square >;

template < typename SampleType >
using Triangle = OscSynth< SampleType, osc::Triangle >;

template < typename SampleType >
using SuperSaw = OscSynth< SampleType, osc::SuperSaw >;

}  // namespace synth


}  // namespace bav::dsp
