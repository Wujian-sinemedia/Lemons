/*
 bv_FFT: This is a wrapper class around several platform-specific FFT implementations that can be used in a generalized way.
 This is packaged as a separate JUCE module from bv_SharedCode, but has a dependancy on bv_SharedCode.
 
 On Apple platforms, this class uses Apple's vDSP framework. On Intel platforms, Intel IPP is used if available.
 */


/*******************************************************************************
 BEGIN_JUCE_MODULE_DECLARATION
 ID:                 bv_FFT
 vendor:             Ben Vining
 version:            0.0.1
 name:               bv_FFT
 description:        wrapper class providing a common interface for various platform-optimized FFT implementations
 dependencies:       bv_SharedCode
 END_JUCE_MODULE_DECLARATION
 *******************************************************************************/


#pragma once

#include "bv_SharedCode/bv_SharedCode.h"


#ifndef BV_USE_FFTW
  #define BV_USE_FFTW 0
#endif

#ifndef BV_USE_KISSFFT
  #define BV_USE_KISSFFT 0
#endif



namespace bav::dsp
{
    
    /* abstract base class that defines the interface for the FFT implementations.  */
    
    class FFTinterface
    {
    public:
        virtual ~FFTinterface() = default;
        
        virtual int getSize() const = 0;
        
        virtual void initFloat() = 0;
        virtual void initDouble() = 0;
        
        virtual bool isFloatInitialized() = 0;
        virtual bool isDoubleInitialized() = 0;
        
        virtual void forward (const double* BV_R_ realIn, double* BV_R_ realOut, double* BV_R_ imagOut) = 0;
        virtual void forwardInterleaved (const double* BV_R_ realIn, double* BV_R_ complexOut) = 0;
        virtual void forwardPolar (const double* BV_R_ realIn, double* BV_R_ magOut, double* BV_R_ phaseOut) = 0;
        virtual void forwardMagnitude (const double* BV_R_ realIn, double* BV_R_ magOut) = 0;
        
        virtual void forward (const float* BV_R_ realIn, float* BV_R_ realOut, float* BV_R_ imagOut) = 0;
        virtual void forwardInterleaved (const float* BV_R_ realIn, float* BV_R_ complexOut) = 0;
        virtual void forwardPolar (const float* BV_R_ realIn, float* BV_R_ magOut, float* BV_R_ phaseOut) = 0;
        virtual void forwardMagnitude (const float* BV_R_ realIn, float* BV_R_ magOut) = 0;
        
        virtual void inverse (const double* BV_R_ realIn, const double* BV_R_ imagIn, double* BV_R_ realOut) = 0;
        virtual void inverseInterleaved (const double* BV_R_ complexIn, double* BV_R_ realOut) = 0;
        virtual void inversePolar (const double* BV_R_ magIn, const double* BV_R_ phaseIn, double* BV_R_ realOut) = 0;
        virtual void inverseCepstral (const double* BV_R_ magIn, double* BV_R_ cepOut) = 0;
        
        virtual void inverse (const float* BV_R_ realIn, const float* BV_R_ imagIn, float* BV_R_ realOut) = 0;
        virtual void inverseInterleaved (const float* BV_R_ complexIn, float* BV_R_ realOut) = 0;
        virtual void inversePolar (const float* BV_R_ magIn, const float* BV_R_ phaseIn, float* BV_R_ realOut) = 0;
        virtual void inverseCepstral (const float* BV_R_ magIn, float* BV_R_ cepOut) = 0;
    };
    
    
}  // namespace
    
    
    /*
     */


/*
    When you compile this juce module, there will only be ONE version of an "FFT" class defined, and each instance of bav::dsp::FFT will use the same implementation.
    (Note that "FFT" is NOT a template class, and every instance of the class has methods for both float and double operations.)
 
    Here is a brief and generalized overview of the various implementations:
 
    - FFTW: Fastest open source library, and portable, but its bulk and GPL licence may be an issue. Supports any FFT length.
 
    - Apple vDSP: Faster than any open source library on Apple hardware, and ships with the OS. There is pretty much never a good reason not to use this if it is available to you, which is why it is the default choice if you are on an Apple platform. Only supports power-of-two FFT lengths.
 
    - Intel IPP (Integrated Performance Primitives): By far the fastest on actual Intel hardware. Of uncertain benefit with other manufacturers. Not available beyond x86/amd64, not open source. Only supports power-of-two FFT lengths.
 
    - Ne10: Decently optimized for NEON processors, but only supports single-precision processing. Calls to the functions with double arguments will internally convert data to 32-bit floating point and back, so some precision may be lost. Only supports power-of-two FFT lengths.
 
    - KissFFT: Only supports single-precision processing internally. Not especially fast. Supports FFT lengths that are any multiple of two.
 
    - Fallback: Plain C++ version hard-coded into the module. Only use this if all else fails. Should produce accurate results, but I make no promises for speed.
*/


#if BV_USE_FFTW  // if someone's gone to the trouble to link to FFTW, they probably want to use it...
  #include "implementations/fft_fftw.h"
#elif BV_USE_VDSP // besides FFTW, the default choice is vDSP if it's available to us
  #include "implementations/fft_vdsp.h"
#elif BV_USE_IPP  // next best is IPP
  #include "implementations/fft_ipp.h"
#elif BV_USE_NE10  // next best is Ne10
  #include "implementations/fft_ne10.h"
#elif BV_USE_KISSFFT // next best is KissFFT
  #include "implementations/fft_kissfft.h"
#else
  #include "implementations/fft_fallback.h"
#endif
    

