#pragma once

/*-------------------------------------------------------------------------------------

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                 lemons_midi
 vendor:             Ben Vining
 version:            0.0.1
 name:               lemons_midi
 description:        Midi utilities
 dependencies:       lemons_core mts_esp_cpp

 END_JUCE_MODULE_DECLARATION

-------------------------------------------------------------------------------------*/


#include "MidiUtilities/MidiFIFO.h"
#include "MidiUtilities/MidiUtilities.h"
#include "MidiUtilities/PitchbendTracker.h"

#include "processors/MidiProcessor.h"
#include "processors/MidiChoppingProcessor/MidiChoppingProcessor.h"

#include "PitchConverter/PitchConverter.h"

#include "MidiUtilities/PitchPipeline.h"
