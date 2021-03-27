
/*
    This file defines some useful platform macros and provides some useful system functions.
*/


/*
     These conditionals declare the macros
     - BV_WINDOWS
     - BV_ANDROID
     - BV_LINUX
     - BV_APPLE
     - BV_OSX
     - BV_IOS
     - BV_BSD
     - BV_POSIX
     ...based on the current operating system.
 Each macro will be defined to either 1 or 0, so you should reference them by using #if... ,  NOT  #ifdef... !!!
 
     It also declares a string literal macro BV_OPERATING_SYSTEM_NAME which can be used if you need a text description of the OS.
 */


#if defined (_WIN32) || defined (_WIN64)
  #define  BV_WINDOWS 1
  #define  BV_OPERATING_SYSTEM_NAME "Windows"
#elif __ANDROID__
  #define  BV_ANDROID 1
  #define  BV_OPERATING_SYSTEM_NAME "Android"
#elif defined (LINUX) || defined (__linux__)
  #define  BV_LINUX 1
  #define  BV_OPERATING_SYSTEM_NAME "Linux"
#elif __APPLE__
  #define BV_APPLE 1
  #include <TargetConditionals.h>
  #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
    #define  BV_IOS 1
    #define  BV_OPERATING_SYSTEM_NAME "iOS"
  #else
    #define  BV_OSX 1
    #define  BV_OPERATING_SYSTEM_NAME "OSX"
  #endif
#elif defined (__FreeBSD__) || (__OpenBSD__)
  #define  BV_BSD 1
  #define  BV_OPERATING_SYSTEM_NAME "BSD"
#elif defined (_POSIX_VERSION)
  #define  BV_POSIX 1
  #define  BV_OPERATING_SYSTEM_NAME "Posix"
#else
  #warning "Unknown platform!"
#endif

#ifndef BV_WINDOWS
  #define BV_WINDOWS 0
#endif

#ifndef BV_ANDROID
  #define BV_ANDROID 0
#endif

#ifndef BV_LINUX
  #define BV_LINUX 0
#endif

#ifndef BV_APPLE
  #define BV_APPLE 0
#endif

#ifndef BV_IOS
  #define BV_IOS 0
#endif

#ifndef BV_OSX
  #define BV_OSX 0
#endif

#ifndef BV_BSD
  #define BV_BSD 0
#endif

#ifndef BV_POSIX
  #if BV_OSX || BV_LINUX || BV_BSD
    #define BV_POSIX 1
  #else
    #define BV_POSIX 0
  #endif
#endif

#ifndef BV_OPERATING_SYSTEM_NAME
  #define BV_OPERATING_SYSTEM_NAME ""
#endif



/*
    These conditionals select whether to use any optimization libraries for vecops, FFTs, etc.
    Changing these options here will control which implementation is used for the entire Shared-code module, including vecops and the FFT module.
    - On Apple platforms, Accelerate/vDSP is included with the OS. It should always be available, and there's pretty much no reason not to use it.
    - "IPP" stands for Intel Integrated Performance Primitives, which is available on Intel Atom, Core, and Xeon processors. IPP must be specially installed and linked to in order to use it.
    - Ne10 is an open-source library of vectorized functions for ARM NEON processors. It must be specially compiled and linked to in order to use it.
*/

#if BV_APPLE
  #define BV_USE_VDSP 1
  #include <Accelerate/Accelerate.h>
#else
  #define BV_USE_VDSP 0
#endif

#ifndef JUCE_USE_VDSP_FRAMEWORK
  #define JUCE_USE_VDSP_FRAMEWORK BV_USE_VDSP
#endif

#ifndef BV_USE_IPP
  #define BV_USE_IPP 0
#endif

#if BV_USE_IPP
  #include <ippversion.h>
  #include <ipps.h>
#endif

#ifndef BV_USE_NE10
  #define BV_USE_NE10 0
#endif

#if BV_USE_NE10
  #include <NE10.h>
#endif


#ifndef BV_USE_FFTW
  #define BV_USE_FFTW 0
#endif


/*
    These conditionals declare the following macros:
 
    - BV_POSIX_MEMALIGN
    - BV_MALLOC_IS_ALIGNED
    - BV_HAVE__ALIGNED_MALLOC
*/

#if BV_POSIX || BV_LINUX || BV_OSX
  #define BV_POSIX_MEMALIGN 1
#else
  #define BV_POSIX_MEMALIGN 0
#endif

#ifndef MALLOC_IS_NOT_ALIGNED
  #if BV_APPLE
    #define BV_MALLOC_IS_ALIGNED 1
  #endif
#endif

#ifndef BV_MALLOC_IS_ALIGNED
  #define BV_MALLOC_IS_ALIGNED 0
#endif

#ifndef LACK__ALIGNED_MALLOC
  #if BV_WINDOWS
    #define BV_HAVE__ALIGNED_MALLOC 1
  #endif
#endif

#ifndef BV_HAVE__ALIGNED_MALLOC
  #define BV_HAVE__ALIGNED_MALLOC 0
#endif


#if BV_POSIX_MEMALIGN
  #include <sys/mman.h>
#endif


/*
    These conditionals set the macro BV_BIT_DEPTH to either 32 or 64.
*/

#if BV_WINDOWS
  #ifdef _MSC_VER
    #ifdef _WIN64
      #define BV_BIT_DEPTH 64
    #else
      #define BV_BIT_DEPTH 32
    #endif
  #endif

  #ifdef __MINGW32__
    #ifdef __MINGW64__
      #define BV_BIT_DEPTH 64
    #else
      #define BV_BIT_DEPTH 32
    #endif
  #endif
#endif  /* if BV_WINDOWS  */

#if BV_APPLE
  #ifdef __LP64__
    #define BV_BIT_DEPTH 64
  #else
    #define BV_BIT_DEPTH 32
  #endif
#endif  /* if BV_APPLE  */

#if BV_LINUX || JUCE_ANDROID
  #if defined (__LP64__) || defined (_LP64) || defined (__arm64__)
    #define BV_BIT_DEPTH 64
  #else
    #define BV_BIT_DEPTH 32
  #endif
#endif  /* if BV_LINUX || JUCE_ANDROID  */

#ifndef BV_BIT_DEPTH
  #define BV_BIT_DEPTH 32
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



namespace bav
{

/*
    this function attempts to return the default location your plugin's preset files should be saved to and loaded from.
    if the directory cannot be found for your plugin, calling this function will attempt to create it.
 */
static inline juce::File getPresetsFolder (std::string companyName, std::string pluginName)
{
    juce::File rootFolder;
    
#if BV_WINDOWS
    rootFolder = juce::File::getSpecialLocation (juce::File::SpecialLocationType::userDocumentsDirectory);
#else
    rootFolder = juce::File::getSpecialLocation (juce::File::SpecialLocationType::userApplicationDataDirectory);
  #if BV_OSX
    rootFolder = rootFolder.getChildFile ("Audio").getChildFile ("Presets");
  #endif
#endif
    
    rootFolder = rootFolder.getChildFile (companyName).getChildFile (pluginName);
    
    if (! rootFolder.isDirectory())
        rootFolder.createDirectory(); // creates the presets folder if it doesn't already exist
    
    return rootFolder;
}
    
    
// simple function to silence "unused variable" warnings
template <typename... Args>
BV_FORCE_INLINE void unused(Args&&...) {}

    
}  // namespace
