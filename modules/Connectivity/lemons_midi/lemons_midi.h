#pragma once

/*-------------------------------------------------------------------------------------

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                 lemons_midi
 vendor:             Ben Vining
 version:            0.0.1
 name:               lemons_midi
 description:        Midi utilities
 dependencies:       lemons_serializing mts_esp_cpp

 END_JUCE_MODULE_DECLARATION

-------------------------------------------------------------------------------------*/


#include "utilities/MidiFIFO.h"
#include "utilities/MidiUtilities.h"
#include "utilities/VelocityHelper.h"
#include "utilities/PitchbendTracker.h"

#include "processors/MidiProcessor.h"
#include "processors/MidiChoppingProcessor.h"

#include "utilities/PitchPipeline.h"

#include "utilities/MidiKeyboardState.h"
