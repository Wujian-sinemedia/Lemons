/*******************************************************************************
 BEGIN_JUCE_MODULE_DECLARATION
 ID:                 bv_midi
 vendor:             Ben Vining
 version:            0.0.1
 name:               bv_midi
 description:        Midi utilities
 dependencies:       bv_core
 END_JUCE_MODULE_DECLARATION
 *******************************************************************************/

#pragma once

#include <bv_core/bv_core.h>

#include "MidiUtilities/MidiFIFO.h"
#include "MidiUtilities/MidiUtilities.h"
#include "MidiUtilities/PitchbendTracker.h"
#include "MidiUtilities/PitchConverter.h"
#include "MidiUtilities/VelocityHelper.h"
