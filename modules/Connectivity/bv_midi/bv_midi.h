#pragma once

/*-------------------------------------------------------------------------------------

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                 bv_midi
 vendor:             Ben Vining
 version:            0.0.1
 name:               bv_midi
 description:        Midi utilities
 dependencies:       bv_core

 END_JUCE_MODULE_DECLARATION

-------------------------------------------------------------------------------------*/


#include "MidiUtilities/MidiFIFO.h"
#include "MidiUtilities/MidiUtilities.h"
#include "MidiUtilities/PitchbendTracker.h"

#include "processors/MidiProcessor.h"
#include "processors/MidiChoppingProcessor/MidiChoppingProcessor.h"

#include "PitchConverter/PitchConverter.h"

#include "MidiUtilities/PitchPipeline.h"
