#pragma once

#if 0

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                 bv_midi
 vendor:             Ben Vining
 version:            0.0.1
 name:               bv_midi
 description:        Midi utilities
 dependencies:       bv_core bv_mts_esp_client

 END_JUCE_MODULE_DECLARATION
 
#endif


#include <bv_core/bv_core.h>
#include <bv_mts_esp_client/bv_mts_esp_client.h>

#include "MidiUtilities/MidiFIFO.h"
#include "MidiUtilities/MidiUtilities.h"
#include "MidiUtilities/PitchbendTracker.h"
#include "MidiUtilities/PitchPipeline.h"

#include "processors/MidiProcessor.h"
