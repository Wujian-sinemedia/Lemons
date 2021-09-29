#pragma once

/*-------------------------------------------------------------------------------------

BEGIN_JUCE_MODULE_DECLARATION

ID:                 lemons_synth
vendor:             Ben Vining
version:            0.0.1
name:               lemons_synth
description:        Infrastructure for a polyphonic synthesizer instrument
dependencies:       lemons_audio_effects lemons_oscillators

END_JUCE_MODULE_DECLARATION
 
 -------------------------------------------------------------------------------------*/


namespace lemons::dsp
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

}  // namespace lemons::dsp


#include "SynthVoice/SynthVoice.h"
#include "Synth/Synth.h"

#include "BasicSynths/LambdaSynth.h"
#include "BasicSynths/TemplateSynth.h"
#include "BasicSynths/OscSynth.h"
