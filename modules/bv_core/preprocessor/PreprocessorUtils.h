
#pragma once

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


/*---------------------------------------------------------------------------*/

/*
 Cross-platform "force inline" macro
 */

#if JUCE_WINDOWS
#define BV_FORCE_INLINE __forceinline
#else
#define BV_FORCE_INLINE inline __attribute__ ((always_inline))
#endif


/*---------------------------------------------------------------------------*/

/*
 Cross-platform macro that creates a CPU wait/sleep inctruction
 */

#if JUCE_INTEL
#define BV_WAIT_INSTRUCTION _mm_pause()
#else
#define BV_WAIT_INSTRUCTION __asm__ __volatile__("yield")
#endif
