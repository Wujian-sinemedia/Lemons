/*******************************************************************************
 BEGIN_JUCE_MODULE_DECLARATION
 ID:                           bv_SharedCode
 vendor:                    Ben Vining
 version:                    0.0.1
 name:                      Ben Vining's codebase
 description:              General utilities useful for developing plugins.
 dependencies:         juce_audio_utils
 OSXFrameworks:    Accelerate
 iOSFrameworks:      Accelerate
 END_JUCE_MODULE_DECLARATION
 *******************************************************************************/


#pragma once


#ifndef BV_HAS_BINARY_DATA
#    define BV_HAS_BINARY_DATA 0
#endif

#include <juce_audio_utils/juce_audio_utils.h>


/*=======================================================================
 These conditionals select whether to use any optimization libraries for vecops, FFTs, etc.
 Changing these options here will control which implementation is used for the entire Shared-code module, including vecops and the FFT module.
 
 - On Apple platforms, Accelerate/vDSP is included with the OS. It should always be available, and there's pretty much no reason not to use it.
 - "IPP" stands for Intel Integrated Performance Primitives, which is available on Intel Atom, Core, and Xeon processors. By far the fastest on actual Intel hardware. IPP must be specially installed and linked to in order to use it.
 - MIPP is an open-source library that serves as a portable wrapper around various SIMD instruction sets. Supports NEON, SSE, AVX and AVX-512. Can be found at https://github.com/aff3ct/MIPP.
 - Ne10 is an open-source library of vectorized functions for ARM NEON processors. Supports fewer architectures than MIPP (will internally revert to C code if compiled using Ne10 and run on a non-NEON processor), but when run on a NEON processor, should outpace MIPP as it is more specially tailored to the various flavors of NEON processors.
=======================================================================*/

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
#    define BV_USE_NE10 0
#endif

#undef JUCE_USE_VDSP_FRAMEWORK
#define JUCE_USE_VDSP_FRAMEWORK BV_USE_VDSP

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


#ifdef __clang__
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Weverything"
#elif defined __GNUC__
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Weverything"
#elif defined _MSC_VER
#    pragma warning(push, 0)
#endif


#if BV_USE_VDSP
#    include <Accelerate/Accelerate.h>
#elif BV_USE_IPP
#    include <ippversion.h>
#    include <ipps.h>
#elif BV_USE_NE10
#    include <NE10.h>
#elif BV_USE_MIPP
#    include <mipp.h>
#endif


#ifdef __clang__
#    pragma clang diagnostic pop
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wunused-function"
#elif defined __GNUC__
#    pragma GCC diagnostic pop
#    pragma GCC diagnostic push "-Wunused-function"
#elif defined _MSC_VER
#    pragma warning(pop)
#endif


// misc
#include "misc/StaticObjects.h"
#include "misc/misc.h"
#include "misc/LockFreeFifo.h"

#include "serializing/SerializableData.h"
#include "serializing/valuetree_json_converter.h"


// files
#include "files/FileUtilities.h"
#include "files/BinaryDataHelpers/BinaryDataHelpers.h"


// localization
#include "localization/localization.h"


#include "misc/SystemInitializer.h"


// async
#include "async/AsyncUtils.h"


// math
#include "math/mathHelpers.h"
#include "math/vecops/vecops.h"
#include "math/intops/intOps.h"


#ifdef __clang__
#    pragma clang diagnostic pop
#elif defined __GNUC__
#    pragma GCC diagnostic pop
#endif
