#pragma once

/*-------------------------------------------------------------------------------------

BEGIN_JUCE_MODULE_DECLARATION

ID:                 lemons_dsp
vendor:             Ben Vining
version:            0.0.1
name:               lemons_dsp
description:        DSP/audio utilities and effects
dependencies:       juce_audio_utils juce_dsp lemons_midi

END_JUCE_MODULE_DECLARATION

 -------------------------------------------------------------------------------------*/


#include "FIFOs/CircularBuffer.h"
#include "FIFOs/AudioFIFO.h"
#include "FIFOs/AudioAndMidiFIFO.h"

#include "engines/AudioEngine.h"
#include "engines/LatencyEngine.h"

#include "filters/Filter.h"

#include "BufferUtils/BufferUtils.h"

#include "PitchDetector/PitchDetector.h"

#include "BasicProcessor/BasicProcessor.h"
