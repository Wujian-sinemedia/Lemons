/*******************************************************************************
 BEGIN_JUCE_MODULE_DECLARATION
 ID:                 bv_SharedCode
 vendor:             Ben Vining
 version:            0.0.1
 name:               Ben Vining's codebase
 description:        General utilities useful for developing plugins.
 dependencies:       juce_audio_utils, juce_dsp
 END_JUCE_MODULE_DECLARATION
 *******************************************************************************/


#pragma once

// dependencies
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_dsp/juce_dsp.h>

// the rest of this module
#include "misc/System.h"

#ifdef __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wunused-function"
#endif

#include "misc/MessageQueue.h"
#include "misc/Parameters.h"
#include "math/mathHelpers.h"
#include "math/intOps.h"
#include "math/VectorOps.h"
#include "midi/MidiFIFO.h"
#include "midi/MidiUtilities.h"
#include "dsp/AudioFIFO.h"
#include "dsp/Panner.h"
#include "dsp/FX/ReorderableFxChain.h"
#include "dsp/FX/NoiseGate.h"
#include "dsp/FX/Compressor.h"
#include "dsp/FX/Limiter.h"
#include "dsp/FX/DeEsser.h"
#include "dsp/FX/Reverb.h"
#include "dsp/FIFOWrappedEngine.h"
#include "gui/gui.h"
#include "data/DataHelpers.h"


#ifdef __clang__
  #pragma clang diagnostic pop
#endif


/*
    This handy macro is a platform independent way of stopping compiler warnings for unused variables.
*/
#ifndef UNUSED_NOWARN
  #if defined(JUCE_MAC) || defined(JUCE_IOS)
    // enable supression of unused variable in GCC
    #define UNUSED_NOWARN __attribute__((unused))
  #elif defined(JUCE_MSVC)
    #define UNUSED_NOWARN
    // disable unused variable warnings in MSVC (Windows)
    #pragma warning( push )
    #pragma warning( disable : 4705 )
  #else
    #define UNUSED_NOWARN
  #endif
#endif

