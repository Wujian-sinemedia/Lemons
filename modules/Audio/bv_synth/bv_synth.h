#pragma once

#if 0

BEGIN_JUCE_MODULE_DECLARATION

ID:                 bv_synth
vendor:             Ben Vining
version:            0.0.1
name:               bv_synth
description:        Infrastructure for a polyphonic synthesizer instrument
dependencies:       bv_audio_effects bv_oscillators

END_JUCE_MODULE_DECLARATION

#endif


namespace bav::dsp
{
template < typename SampleType >
class SynthBase;

namespace synth
{
template < typename SampleType >
class AutomatedHarmonyVoice;

template < typename SampleType >
class PanningManager;

template < typename SampleType >
class MidiManager;
}  // namespace synth

}  // namespace bav::dsp


#include "SynthVoice/SynthVoice.h"
#include "Synth/Synth.h"
#include "BasicSynth/BasicSynth.h"
