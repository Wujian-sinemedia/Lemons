
/*
    This file defines some useful platform macros and tries to set up some appropriate platform-specific features.
*/


#undef BV_POSIX
  #if JUCE_LINUX || JUCE_MAC || JUCE_BSD
    #define BV_POSIX 1
  #else
    #define BV_POSIX 0
  #endif
#endif


/*
    These conditionals select whether to use any optimization libraries for vecops, FFTs, etc.
    Changing these options here will control which implementation is used for the entire Shared-code module, including vecops and the FFT module.
 
    - On Apple platforms, Accelerate/vDSP is included with the OS. It should always be available, and there's pretty much no reason not to use it.
    - "IPP" stands for Intel Integrated Performance Primitives, which is available on Intel Atom, Core, and Xeon processors. By far the fastest on actual Intel hardware. IPP must be specially installed and linked to in order to use it.
    - MIPP is an open-source library that serves as a portable wrapper around various SIMD instruction sets. Supports NEON, SSE, AVX and AVX-512. Can be found at https://github.com/aff3ct/MIPP.
    - Ne10 is an open-source library of vectorized functions for ARM NEON processors. Supports fewer architectures than MIPP (will internally revert to C code if compiled using Ne10 and run on a non-NEON processor), but when run on a NEON processor, should outpace MIPP as it is more specially tailored to the various flavors of NEON processors.
*/


/// vDSP ///

#ifdef BV_IGNORE_VDSP
  #ifdef BV_USE_VDSP
    #undef BV_USE_VDSP
  #endif
#define BV_USE_VDSP 0
#endif

#ifndef BV_USE_VDSP
  #if JUCE_IOS || JUCE_MAC
    #define BV_USE_VDSP 1
  #else
    #define BV_USE_VDSP 0
  #endif
#endif  /* ifndef BV_USE_VDSP */

#if BV_USE_VDSP
  #include <Accelerate/Accelerate.h>

  #ifndef BV_IGNORE_IPP
    #define BV_IGNORE_IPP
  #endif

  #ifndef BV_IGNORE_NE10
    #define BV_IGNORE_NE10
  #endif

  #ifndef BV_IGNORE_MIPP
    #define BV_IGNORE_MIPP
  #endif

#endif /* if BV_USE_VDSP */

#ifdef JUCE_USE_VDSP_FRAMEWORK
  #undef JUCE_USE_VDSP_FRAMEWORK
#endif

#define JUCE_USE_VDSP_FRAMEWORK BV_USE_VDSP


/// IPP ///

#if ! JUCE_INTEL
  #define BV_IGNORE_IPP
#endif

#ifdef BV_IGNORE_IPP
  #ifdef BV_USE_IPP
    #undef BV_USE_IPP
  #endif
#define BV_USE_IPP 0
#endif

#ifndef BV_USE_IPP
  #define BV_USE_IPP 0
#endif

#if BV_USE_IPP
  #include <ippversion.h>
  #include <ipps.h>

  #ifndef BV_IGNORE_NE10
    #define BV_IGNORE_NE10
  #endif

  #ifndef BV_IGNORE_MIPP
    #define BV_IGNORE_MIPP
  #endif

#endif /* if BV_USE_IPP */


/// Ne10 ///

#if ! JUCE_ARM
  #define BV_IGNORE_NE10
#endif

#ifdef BV_IGNORE_NE10
  #ifdef BV_USE_NE10
    #undef BV_USE_NE10
  #endif
#define BV_USE_NE10 0
#endif

#ifndef BV_USE_NE10
  #define BV_USE_NE10 0
#endif

#if BV_USE_NE10
  #include <NE10.h>

  #ifndef BV_IGNORE_MIPP
    #define BV_IGNORE_MIPP
  #endif
#endif


/// MIPP ///

#ifdef BV_IGNORE_MIPP
  #ifdef BV_USE_MIPP
    #undef BV_USE_MIPP
  #endif
#define BV_USE_MIPP 0
#endif

#ifndef BV_USE_MIPP
  #define BV_USE_MIPP 0
#endif

#if BV_USE_MIPP
  #include "mipp.h"
#endif


/*
    These conditionals set the macro BV_BIT_DEPTH to either 32 or 64.
*/

#if JUCE_32BIT
  #define BV_BIT_DEPTH 32
#elif JUCE_64BIT
  #define BV_BIT_DEPTH 64
#else
  #error Unknown bit depth
#endif


/*
    Platform-independant restriction macro to reduce pointer aliasing, allowing for better optimizations. Use with care, this can result in UB!
*/

#ifdef __clang__
  #define BV_R_ __restrict__
#else
  #ifdef __GNUC__
    #define BV_R_ __restrict__
  #endif
#endif

#ifndef BV_R_
  #ifdef _MSC_VER
    #define BV_R_ __restrict
  #else
    #ifdef __MSVC__
      #define BV_R_ __restrict
    #endif
  #endif
#endif

#ifndef BV_R_
  #warning No pointer restriction available for your compiler
  #define BV_R_
#endif



/*
    Cross-platform "force inline" macro
*/

#if BV_WINDOWS
  #define BV_FORCE_INLINE __forceinline
#else
  #define BV_FORCE_INLINE inline __attribute__((always_inline))
#endif

