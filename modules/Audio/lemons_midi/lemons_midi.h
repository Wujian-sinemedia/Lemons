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
 dependencies:       juce_audio_basics

 END_JUCE_MODULE_DECLARATION

-------------------------------------------------------------------------------------*/


#if LEMONS_UNIT_TESTS
#  include <lemons_unit_testing/lemons_unit_testing.h>
#endif


#include "utilities/MidiUtilities.h"
#include "utilities/MidiFIFO.h"
#include "utilities/VelocityHelper.h"

#include "tuning/mts_client.h"
#include "tuning/PitchbendTracker.h"
#include "tuning/PitchPipeline.h"

#include "processors/MidiProcessor.h"
#include "processors/MidiChoppingProcessor.h"
