/*******************************************************************************
 BEGIN_JUCE_MODULE_DECLARATION
 ID:                 bv_midi
 vendor:             Ben Vining
 version:            0.0.1
 name:               bv_midi
 description:        Midi utilities
 dependencies:       juce_audio_basics
 END_JUCE_MODULE_DECLARATION
 *******************************************************************************/

#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

#include "MidiUtilities/MidiFIFO.h"
#include "MidiUtilities/MidiUtilities.h"
#include "MidiUtilities/PitchbendTracker.h"
#include "MidiUtilities/PitchConverter.h"
#include "MidiUtilities/VelocityHelper.h"
#include "MidiUtilities/Chord.h"
