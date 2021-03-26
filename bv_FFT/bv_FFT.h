/*
    bv_FFT: This is a wrapper class around several platform-specific FFT implementations that can be used in a generalized way.
    This is packaged as a separate JUCE module from bv_SharedCode, but has a dependancy on bv_SharedCode.
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

#if INTEL_IPP && ! BV_USE_VDSP
  #include "ipps.h"
#endif


namespace bav::dsp
{
    

//template typename<SampleType>
class FFT
{
public:
    FFT(int numBits)
#if BV_USE_VDSP
        : bits(vDSP_Length(numBits)),
          setup (vDSP_create_fftsetup(bits, 2)),
          size(1 << bits)
#elif INTEL_IPP
        : size_(1 << bits)
#endif
    {
#if INTEL_IPP
        int spec_size = 0;
        int spec_buffer_size = 0;
        int buffer_size = 0;
        ippsFFTGetSize_R_32f (bits, IPP_FFT_DIV_INV_BY_N, ippAlgHintNone, &spec_size, &spec_buffer_size, &buffer_size);
        
        spec = std::make_unique<Ipp8u[]>(spec_size);
        spec_buffer = std::make_unique<Ipp8u[]>(spec_buffer_size);
        buffer = std::make_unique<Ipp8u[]>(buffer_size);
        
        ippsFFTInit_R_32f (&ipp_specs, bits, IPP_FFT_DIV_INV_BY_N, ippAlgHintNone, spec.get(), spec_buffer.get());
#endif
    }
    
    
    ~FFT()
    {
#if BV_USE_VDSP
        vDSP_destroy_fftsetup (setup);
#endif
    }
    
    
    void transformRealForward(float* data)
    {
#if BV_USE_VDSP
        static const float kMult = 0.5f;
        data[size] = 0.0f;
        DSPSplitComplex split = { data, data + 1 };
        vDSP_fft_zrip (setup, &split, 2, bits, kFFTDirection_Forward);
        vDSP_vsmul (data, 1, &kMult, data, 1, size);
        
        data[size] = data[1];
        data[size + 1] = 0.0f;
        data[1] = 0.0f;
#elif INTEL_IPP
        data[size] = 0.0f;
        ippsFFTFwd_RToPerm_32f_I ((Ipp32f*)data, ipp_specs, buffer.get());
        data[size] = data[1];
        data[size + 1] = 0.0f;
        data[1] = 0.0f;
#endif
    }
    
    
    void transformRealInverse(float* data)
    {
#if BV_USE_VDSP
        float multiplier = 1.0f / size;
        DSPSplitComplex split = { data, data + 1 };
        data[1] = data[size];
        
        vDSP_fft_zrip (setup, &split, 2, bits, kFFTDirection_Inverse);
        vDSP_vsmul (data, 1, &multiplier, data, 1, size * 2);
        memset (data + size, 0, size * sizeof(float));
#elif INTEL_IPP
        data[1] = data[size];
        ippsFFTInv_PermToR_32f_I ((Ipp32f*)data, ipp_specs, buffer.get());
        memset (data + size, 0, size_ * sizeof(float));
#endif
    }
    
    
    
private:
    
#if BV_USE_VDSP
    
    vDSP_Length bits;
    vDSP_Length size;
    FFTSetup setup;
    
#elif INTEL_IPP
    
    int size;
    IppsFFTSpec_R_32f* ipp_specs;
    std::unique_ptr<Ipp8u[]> spec;
    std::unique_ptr<Ipp8u[]> spec_buffer;
    std::unique_ptr<Ipp8u[]> buffer;
    
#endif
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FFT)
};
    

/// explicit instantiations...
//template class FFT<float>;
//template class FFT<double>;


}  // namespace
