#pragma once

#if 0

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                bv_SharedCode
 vendor:            Ben Vining
 version:           0.0.1
 name:              bv_SharedCode
 description:       General utilities useful for developing plugins.
 dependencies:      juce_audio_utils
 OSXFrameworks:     Accelerate
 iOSFrameworks:     Accelerate

 END_JUCE_MODULE_DECLARATION

#endif


//==============================================================================
/** Config: BV_HAS_BINARY_DATA
 
    Set this to 1 if your project has a juce binary data target.
 */
#ifndef BV_HAS_BINARY_DATA
#    define BV_HAS_BINARY_DATA 0
#endif


//==============================================================================
/** Config: BV_USE_VDSP
 
    Set this to 1 to use Apple's vDSP library for vecops SIMD.
    vDSP ships with the OS on Mac and iOS, which is why it's the default on Apple platforms.
    (Setting this to 1 disables IPP, Ne10 and MIPP.)
 */
#ifndef BV_USE_VDSP
#    if JUCE_IOS || JUCE_MAC
#        define BV_USE_VDSP 1
#    else
#        define BV_USE_VDSP 0
#    endif
#endif

#if BV_USE_VDSP
#    undef BV_USE_IPP
#    undef BV_USE_NE10
#    undef BV_USE_MIPP
#    define BV_USE_IPP  0
#    define BV_USE_NE10 0
#    define BV_USE_MIPP 0
#endif

#undef JUCE_USE_VDSP_FRAMEWORK
#define JUCE_USE_VDSP_FRAMEWORK BV_USE_VDSP


//==============================================================================
/** Config: BV_USE_IPP
 
    Set this to 1 to use Intel's Integrated Performance Primitives library for vecops SIMD.
    (Setting this to 1 disables vDSP, Ne10 and MIPP.)
 */
#if ! JUCE_INTEL
#    undef BV_USE_IPP
#    define BV_USE_IPP 0
#endif

#ifndef BV_USE_IPP
#    define BV_USE_IPP 0
#endif

#if BV_USE_IPP
#    undef BV_USE_NE10
#    undef BV_USE_MIPP
#    define BV_USE_NE10 0
#    define BV_USE_MIPP 0
#endif


//==============================================================================
/** Config: BV_USE_NE10
 
    Ne10 is an open-source SIMD intrinsics library for Arm NEON. Set this to 1 to use Ne10 for vecops SIMD.
    (Setting this to 1 disables vDSP, IPP and MIPP.)
 */
#ifndef __ARM_NEON__
#    undef BV_USE_NE10
#    define BV_USE_NE10 0
#endif

#ifndef BV_USE_NE10
#    define BV_USE_NE10 0
#endif

#if BV_USE_NE10
#    undef BV_USE_MIPP
#    define BV_USE_MIPP 0
#endif


//==============================================================================
/** Config: BV_USE_MIPP
 
    MIPP is a cross-platform SIMD intrinsics library that supports NEON, SSE, AVX and AVX-512.
    Set this to 1 to use MIPP for vecops SIMD.
    (Setting this to 1 disables vDSP, IPP and Ne10.)
 */
#ifndef BV_USE_MIPP
#    define BV_USE_MIPP 0
#endif


/*=======================================================================*/
/*
 Platform-independant restriction macro to reduce pointer aliasing, allowing for better optimizations. Use with care, this can result in UB!
 */

#if defined __clang__ || defined __GNUC__
#    define BV_R_ __restrict__
#else
#    if defined _MSC_VER || defined __MSVC__
#        define BV_R_ __restrict
#    endif
#endif

#ifndef BV_R_
#    warning No pointer restriction available for your compiler
#    define BV_R_
#endif

/*=======================================================================*/
/*
 Cross-platform "force inline" macro
 */

#if JUCE_WINDOWS
#    define BV_FORCE_INLINE __forceinline
#else
#    define BV_FORCE_INLINE inline __attribute__ ((always_inline))
#endif

/*=======================================================================*/
/*
 Cross-platform macro that creates a CPU wait/sleep inctruction
 */

#if JUCE_INTEL
#    define BV_WAIT_INSTRUCTION _mm_pause()
#else
#    define BV_WAIT_INSTRUCTION __asm__ __volatile__("yield")
#endif

/*=======================================================================*/


#include <juce_audio_utils/juce_audio_utils.h>


namespace bav
{
using juce::String;
using juce::ValueTree;
using juce::File;
}


#include "misc/misc.h"
#include "misc/LockFreeFifo.h"
#include "misc/ValueSmoother.h"

#include "events/Broadcaster.h"
#include "events/Listener.h"
#include "events/Timers.h"

#include "serializing/SerializableData/SerializableData.h"
#include "serializing/SerializableData/Specializations.h"
#include "serializing/ValueTreeToJson/valuetree_json_converter.h"
#include "serializing/Serializing/Serializing.h"

#include "files/FileUtilities.h"

#include "binary_data/BinaryDataHelpers.h"

#include "localization/localization.h"

#include "math/mathHelpers.h"
#include "math/vecops/vecops.h"
#include "math/intops/intOps.h"

#include "undo/UndoManager.h"

#include "presets/PresetManager.h"
