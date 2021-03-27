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
 description:        Ben Vining's wrapper class for FFT's
 dependencies:       bv_SharedCode
 END_JUCE_MODULE_DECLARATION
 *******************************************************************************/


#pragma once

#include "bv_SharedCode/bv_SharedCode.h"


namespace bav::dsp
{
    
    /* abstract base class that defines the interface for any of the FFT implementations.  */
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


#if BV_USE_FFTW  // if someone's gone to the trouble to link to FFTW, they probably want to use it...
  #include "fft_fftw.h"
#elif BV_USE_VDSP
  #include "fft_vdsp.h"
#elif BV_USE_IPP
  #include "fft_ipp.h"
#else
  #include "fft_fallback.h"
#endif
    

