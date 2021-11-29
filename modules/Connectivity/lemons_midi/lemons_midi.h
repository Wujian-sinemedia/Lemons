#pragma once

/*-------------------------------------------------------------------------------------

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                 lemons_midi
 vendor:             Lemons
 version:            0.0.1
 name:               lemons_midi
 description:        Midi utilities
 website:            http://benthevining.github.io/Lemons/
 license:            GPL-3.0
 minimumCppStandard: 17
 dependencies:       mts_esp_cpp juce_audio_basics

 END_JUCE_MODULE_DECLARATION

-------------------------------------------------------------------------------------*/


#include "utilities/MidiUtilities.h"
#include "utilities/VelocityHelper.h"
#include "utilities/PitchbendTracker.h"

#include "processors/MidiProcessor.h"
#include "processors/MidiChoppingProcessor.h"

#include "utilities/PitchPipeline.h"

#include "utilities/MidiKeyboardState.h"
