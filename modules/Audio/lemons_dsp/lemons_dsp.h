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
dependencies:       lemons_midi juce_audio_formats

END_JUCE_MODULE_DECLARATION

 -------------------------------------------------------------------------------------*/

//==============================================================================
/** Config: LEMONS_DSP_ENGINE_FACTORY
 
 Set this to 1 to generate the static type registry of engines. You can turn this off to save performance and compile times.
 */
#ifndef LEMONS_DSP_ENGINE_FACTORY
  #define LEMONS_DSP_ENGINE_FACTORY 0
#endif

//==============================================================================


#include "unit_tests/dsp_test.h"
#include "unit_tests/dsp_utils.h"

#include "fifos/CircularBuffer.h"
#include "fifos/AudioFifo.h"
#include "fifos/AudioAndMidiFIFO.h"

#include "engines/AudioEngine.h"
#include "engines/LatencyEngine.h"
#include "engines/Factory.h"
#include "engines/DspProtection.h"
#include "engines/EngineChain.h"

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

#include "audio_files/AudioFile.h"
#include "audio_files/AudioFilePlayer.h"
