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
dependencies:       juce_audio_processors lemons_midi lemons_binaries

END_JUCE_MODULE_DECLARATION

 -------------------------------------------------------------------------------------*/


//==============================================================================
/** Config: LEMONS_USE_VDSP

    Set this to 1 to use Apple's vDSP library for vecops SIMD.
    vDSP ships with the OS on Mac and iOS, which is why it's the default on Apple platforms.
    The Lemons repo's cmake scripts set this automatically for you; if that's how you added the Lemons package, then you don't need to worry about setting this manually.
 */
#ifndef LEMONS_USE_VDSP
#  if JUCE_IOS || JUCE_MAC
#   define LEMONS_USE_VDSP 1
#  else
#   define LEMONS_USE_VDSP 0
#  endif
#endif

/*=======================================================================*/


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

#include "engines/OscEngine.h"

#include "oscillators/choosable.h"

#include "filters/Filter.h"

#include "util/vecops.h"
#include "util/serializing.h"
#include "util/ValueSmoother.h"
#include "util/BufferUtils.h"
#include "util/WhiteNoiseGenerator.h"
#include "util/BasicProcessor.h"
#include "util/MonoStereoConverter.h"
#include "util/PitchDetector.h"
