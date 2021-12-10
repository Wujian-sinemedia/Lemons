#pragma once

/*-------------------------------------------------------------------------------------

BEGIN_JUCE_MODULE_DECLARATION

ID:                 lemons_dsp
vendor:             Lemons
version:            0.0.1
name:               lemons_dsp
description:        DSP/audio utilities and effects
website:            http://benthevining.github.io/Lemons/
license:            GPL-3.0
minimumCppStandard: 17
dependencies:       juce_audio_processors lemons_midi lemons_core

END_JUCE_MODULE_DECLARATION

 -------------------------------------------------------------------------------------*/


#if LEMONS_UNIT_TESTS
#  include <lemons_unit_testing/lemons_unit_testing.h>
#endif


#include "aliases.h"

#include "fifos/CircularBuffer.h"
#include "fifos/AudioFifo.h"
#include "fifos/AudioAndMidiFIFO.h"

#include "engines/AudioEngine.h"
#include "engines/LatencyEngine.h"
#include "engines/AudioFilePlayer.h"
#include "engines/EngineChain.h"
#include "engines/DspProtection.h"

#include "oscillators/oscillators.h"
#include "oscillators/SuperSaw.h"
#include "oscillators/OscEngine.h"
#include "oscillators/choosable.h"

#include "filters/Filter.h"

#include "util/BufferUtils.h"
#include "util/WhiteNoiseGenerator.h"
#include "util/BasicProcessor.h"
#include "util/MonoStereoConverter.h"
#include "util/PitchDetector.h"
