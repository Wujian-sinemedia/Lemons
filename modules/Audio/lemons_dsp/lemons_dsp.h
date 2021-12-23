/*
 ======================================================================================
 
 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 
 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 
 ======================================================================================
 */


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
dependencies:       lemons_binaries juce_audio_processors

END_JUCE_MODULE_DECLARATION

 -------------------------------------------------------------------------------------*/


#if LEMONS_UNIT_TESTS
#  include <lemons_unit_testing/lemons_unit_testing.h>
#endif


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

#include "util/serializing.h"
#include "util/ValueSmoother.h"
#include "util/BufferUtils.h"
#include "util/WhiteNoiseGenerator.h"
#include "util/MonoStereoConverter.h"
#include "util/PitchDetector.h"
