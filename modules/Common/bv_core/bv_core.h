#pragma once

#if 0

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                bv_core
 vendor:            Ben Vining
 version:           0.0.1
 name:              bv_core
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
#    undef BV_USE_MIPP
#    define BV_USE_MIPP 0
#endif

#undef JUCE_USE_VDSP_FRAMEWORK
#define JUCE_USE_VDSP_FRAMEWORK BV_USE_VDSP


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

#define BV_DECLARE_RECURSIVE_VARIADIC_FUNCTION(funcName, Type) \
    void funcName (Type);                                      \
    template < typename... Args >                              \
    void funcName (Type first, Args&&... rest)                 \
    {                                                          \
        funcName (first);                                      \
        funcName (std::forward< Args > (rest)...);             \
    }

/*=======================================================================*/


#include <juce_audio_utils/juce_audio_utils.h>


namespace bav
{
using juce::File;
using juce::String;
using juce::ValueTree;
}  // namespace bav


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

#include "undo/UndoManager.h"
