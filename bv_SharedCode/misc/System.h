
/*
    This file defines some useful platform macros and provides some useful data serialisation functions.
*/


/*
     These conditionals declare the macros
     - BV_WINDOWS
     - BV_ANDROID
     - BV_LINUX
     - BV_OSX
     - BV_IOS
     ...based on the current operating system.
 Each macro will be defined to either 1 or 0, so you should reference them by using #if... ,  NOT  #ifdef... !!!
 
     It also declares a string literal macro BV_OPERATING_SYSTEM_NAME
    which can be used if you need a text description of the OS.
 
    If you are compiling for an Apple target platform, this file will include Apple's vDSP header and vDSP functions will be used where possible in the VectorOps.h file.
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
  #define BV_POSIX 0
#endif


#if (BV_APPLE)
  #define BV_VECTOROPS_USE_VDSP 1
  #include <Accelerate/Accelerate.h>
#else
  #define BV_VECTOROPS_USE_VDSP 0
#endif


/*
    It's never a smart idea to include any C headers before your C++ ones, as they often pollute your namespace with all kinds of dangerous macros like these ones. These symbols are undef'ed here just in case.
 */
#undef max
#undef min

