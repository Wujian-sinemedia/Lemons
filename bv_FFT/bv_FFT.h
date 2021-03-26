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
        virtual ~FFTinterface = default;
        
        virtual int getSize() const = 0;
        
        virtual void initFloat() = 0;
        virtual void initDouble() = 0;
        
        virtual void forward (const double* realIn, double* realOut, double* imagOut) = 0;
        virtual void forwardInterleaved (const double* realIn, double* complexOut) = 0;
        virtual void forwardPolar (const double* realIn, double* magOut, double* phaseOut) = 0;
        virtual void forwardMagnitude (const double* realIn, double* magOut) = 0;
        
        virtual void forward (const float* realIn, float* realOut, float* imagOut) = 0;
        virtual void forwardInterleaved (const float* realIn, float* complexOut) = 0;
        virtual void forwardPolar (const float* realIn, float* magOut, float* phaseOut) = 0;
        virtual void forwardMagnitude (const float* realIn, float* magOut) = 0;
        
        virtual void inverse (const double* realIn, const double* imagIn, double* realOut) = 0;
        virtual void inverseInterleaved (const double* complexIn, double* realOut) = 0;
        virtual void inversePolar (const double* magIn, const double* phaseIn, double* realOut) = 0;
        virtual void inverseCepstral (const double* magIn, double* cepOut) = 0;
        
        virtual void inverse (const float* realIn, const float* imagIn, float* realOut) = 0;
        virtual void inverseInterleaved (const float* complexIn, float* realOut) = 0;
        virtual void inversePolar (const float* magIn, const float* phaseIn, float* realOut) = 0;
        virtual void inverseCepstral (const float* magIn, float* cepOut) = 0;
    };
    
    
    
    /*
    */
    
#if BV_USE_VDSP
    
    class FFT :     public FFTinterface
    {
        using FVO = juce::FloatVectorOperations;
        
    public:
        FFT(int size) :
            m_size(size), m_fspec(0), m_dspec(0),
            m_fpacked(0), m_fspare(0),
            m_dpacked(0), m_dspare(0)
        {
            for (int i = 0; ; ++i) {
                if (m_size & (1 << i)) {
                    m_order = i;
                    break;
                }
            }
        }
        
        ~FFT() override
        {
            if (m_fspec)
            {
                vDSP_destroy_fftsetup (m_fspec);
                aligned_deallocate (m_fspare);
                aligned_deallocate (m_fspare2);
                aligned_deallocate (m_fbuf->realp);
                aligned_deallocate (m_fbuf->imagp);
                delete m_fbuf;
                aligned_deallocate (m_fpacked->realp);
                aligned_deallocate (m_fpacked->imagp);
                delete m_fpacked;
            }
            
            if (m_dspec)
            {
                vDSP_destroy_fftsetupD (m_dspec);
                aligned_deallocate (m_dspare);
                aligned_deallocate (m_dspare2);
                aligned_deallocate (m_dbuf->realp);
                aligned_deallocate (m_dbuf->imagp);
                delete m_dbuf;
                aligned_deallocate (m_dpacked->realp);
                aligned_deallocate (m_dpacked->imagp);
                delete m_dpacked;
            }
        }
        
        
        int getSize() const override
        {
            return m_size;
        }
        
        
        void initFloat() override
        {
            if (m_fspec) return;
            m_fspec = vDSP_create_fftsetup (m_order, FFT_RADIX2);
            m_fbuf = new DSPSplitComplex;
            //!!! "If possible, tempBuffer->realp and tempBuffer->imagp should be 32-byte aligned for best performance."
            m_fbuf->realp = aligned_allocate<float>(m_size);
            m_fbuf->imagp = aligned_allocate<float>(m_size);
            m_fpacked = new DSPSplitComplex;
            m_fpacked->realp = aligned_allocate<float>(m_size / 2 + 1);
            m_fpacked->imagp = aligned_allocate<float>(m_size / 2 + 1);
            m_fspare = aligned_allocate<float>(m_size + 2);
            m_fspare2 = aligned_allocate<float>(m_size + 2);
        }
        
        void initDouble() override
        {
            if (m_dspec) return;
            m_dspec = vDSP_create_fftsetupD (m_order, FFT_RADIX2);
            m_dbuf = new DSPDoubleSplitComplex;
            //!!! "If possible, tempBuffer->realp and tempBuffer->imagp should be 32-byte aligned for best performance."
            m_dbuf->realp = aligned_allocate<double>(m_size);
            m_dbuf->imagp = aligned_allocate<double>(m_size);
            m_dpacked = new DSPDoubleSplitComplex;
            m_dpacked->realp = aligned_allocate<double>(m_size / 2 + 1);
            m_dpacked->imagp = aligned_allocate<double>(m_size / 2 + 1);
            m_dspare = aligned_allocate<double>(m_size + 2);
            m_dspare2 = aligned_allocate<double>(m_size + 2);
        }
        
        
        void forward (const double* realIn, double* realOut, double* imagOut) override
        {
            if (! m_dspec) initDouble();
            packReal (realIn);
            vDSP_fft_zriptD (m_dspec, m_dpacked, 1, m_dbuf, m_order, FFT_FORWARD);
            ddenyq();
            unpackComplex (realOut, imagOut);
        }
        
        void forward (const float* realIn, float* realOut, float* imagOut) override
        {
            if (! m_fspec) initFloat();
            packReal (realIn);
            vDSP_fft_zript (m_fspec, m_fpacked, 1, m_fbuf, m_order, FFT_FORWARD);
            fdenyq();
            unpackComplex (realOut, imagOut);
        }
        
        void forwardInterleaved (const double* realIn, double* complexOut) override
        {
            if (! m_dspec) initDouble();
            packReal (realIn);
            vDSP_fft_zriptD (m_dspec, m_dpacked, 1, m_dbuf, m_order, FFT_FORWARD);
            ddenyq();
            unpackComplex (complexOut);
        }
        
        void forwardInterleaved (const float* realIn, float* complexOut) override
        {
            if (! m_fspec) initFloat();
            packReal(realIn);
            vDSP_fft_zript (m_fspec, m_fpacked, 1, m_fbuf, m_order, FFT_FORWARD);
            fdenyq();
            unpackComplex (complexOut);
        }
        
        void forwardMagnitude (const double* realIn, double* magOut) override
        {
            if (! m_dspec) initDouble();
            packReal (realIn);
            vDSP_fft_zriptD (m_dspec, m_dpacked, 1, m_dbuf, m_order, FFT_FORWARD);
            ddenyq();
            const int hs1 = m_size/2+1;
            vDSP_zvmagsD (m_dpacked, 1, m_dspare, 1, hs1);
            //vvsqrt (m_dspare2, m_dspare, &hs1);
            // vDSP forward FFTs are scaled 2x (for some reason)
            double two = 2.0;
            vDSP_vsdivD (m_dspare2, 1, &two, magOut, 1, hs1);
        }
        
        void forwardMagnitude (const float* realIn, float* magOut) override
        {
            if (! m_fspec) initFloat();
            packReal (realIn);
            vDSP_fft_zript (m_fspec, m_fpacked, 1, m_fbuf, m_order, FFT_FORWARD);
            fdenyq();
            const int hs1 = m_size/2 + 1;
            vDSP_zvmags (m_fpacked, 1, m_fspare, 1, hs1);
            //vvsqrtf (m_fspare2, m_fspare, &hs1);
            // vDSP forward FFTs are scaled 2x (for some reason)
            float two = 2.f;
            vDSP_vsdiv (m_fspare2, 1, &two, magOut, 1, hs1);
        }
        
        void forwardPolar (const double* realIn, double* magOut, double* phaseOut) override
        {
            if (! m_dspec) initDouble();
            const int hs1 = m_size/2+1;
            packReal (realIn);
            vDSP_fft_zriptD (m_dspec, m_dpacked, 1, m_dbuf, m_order, FFT_FORWARD);
            ddenyq();
            // vDSP forward FFTs are scaled 2x (for some reason)
            for (int i = 0; i < hs1; ++i) m_dpacked->realp[i] *= 0.5;
            for (int i = 0; i < hs1; ++i) m_dpacked->imagp[i] *= 0.5;
//            v_cartesian_to_polar (magOut, phaseOut,
//                                  m_dpacked->realp, m_dpacked->imagp, hs1);
        }
        
        void forwardPolar (const float* realIn, float* magOut, float* phaseOut) override
        {
            if (! m_fspec) initFloat();
            const int hs1 = m_size/2+1;
            packReal (realIn);
            vDSP_fft_zript (m_fspec, m_fpacked, 1, m_fbuf, m_order, FFT_FORWARD);
            fdenyq();
            // vDSP forward FFTs are scaled 2x (for some reason)
            for (int i = 0; i < hs1; ++i) m_fpacked->realp[i] *= 0.5f;
            for (int i = 0; i < hs1; ++i) m_fpacked->imagp[i] *= 0.5f;
//            v_cartesian_to_polar (magOut, phaseOut,
//                                  m_fpacked->realp, m_fpacked->imagp, hs1);
        }
        
        /*
        */
        
        void inverse (const double* realIn, const double* imagIn, double* realOut) override
        {
            if (! m_dspec) initDouble();
            packComplex (realIn, imagIn);
            vDSP_fft_zriptD (m_dspec, m_dpacked, 1, m_dbuf, m_order, FFT_INVERSE);
            unpackReal (realOut);
        }
        
        void inverse (const float* realIn, const float* imagIn, float* realOut) override
        {
            if (! m_fspec) initFloat();
            packComplex (realIn, imagIn);
            vDSP_fft_zript (m_fspec, m_fpacked, 1, m_fbuf, m_order, FFT_INVERSE);
            unpackReal (realOut);
        }
        
        void inverseInterleaved (const double* complexIn, double* realOut) override
        {
            if (! m_dspec) initDouble();
            double *d[2] = { m_dpacked->realp, m_dpacked->imagp };
            // v_deinterleave (d, complexIn, 2, m_size/2 + 1);
            vDSP_fft_zriptD (m_dspec, m_dpacked, 1, m_dbuf, m_order, FFT_INVERSE);
            unpackReal (realOut);
        }
        
        void inverseInterleaved (const float* complexIn, float* realOut) override
        {
            if (! m_fspec) initFloat();
            float *f[2] = { m_fpacked->realp, m_fpacked->imagp };
            // v_deinterleave (f, complexIn, 2, m_size/2 + 1);
            vDSP_fft_zript (m_fspec, m_fpacked, 1, m_fbuf, m_order, FFT_INVERSE);
            unpackReal (realOut);
        }
        
        void inversePolar (const double* magIn, const double* phaseIn, double* realOut) override
        {
            if (! m_dspec) initDouble();
            const int hs1 = m_size/2+1;
            // vvsincos (m_dpacked->imagp, m_dpacked->realp, phaseIn, &hs1);
            double *const rp = m_dpacked->realp;
            double *const ip = m_dpacked->imagp;
            for (int i = 0; i < hs1; ++i) rp[i] *= magIn[i];
            for (int i = 0; i < hs1; ++i) ip[i] *= magIn[i];
            dnyq();
            vDSP_fft_zriptD (m_dspec, m_dpacked, 1, m_dbuf, m_order, FFT_INVERSE);
            unpackReal (realOut);
        }
        
        void inversePolar (const float* magIn, const float* phaseIn, float* realOut) override
        {
            if (! m_fspec) initFloat();
            const int hs1 = m_size/2+1;
            // vvsincosf (m_fpacked->imagp, m_fpacked->realp, phaseIn, &hs1);
            float *const rp = m_fpacked->realp;
            float *const ip = m_fpacked->imagp;
            for (int i = 0; i < hs1; ++i) rp[i] *= magIn[i];
            for (int i = 0; i < hs1; ++i) ip[i] *= magIn[i];
            fnyq();
            vDSP_fft_zript (m_fspec, m_fpacked, 1, m_fbuf, m_order, FFT_INVERSE);
            unpackReal (realOut);
        }
        
        void inverseCepstral (const double* magIn, double* cepOut) override
        {
            if (! m_dspec) initDouble();
            const int hs1 = m_size/2 + 1;
            FVO::copy (m_dspare, magIn, hs1);
            FVO::add (m_dspare, 0.000001, hs1);
            // vvlog (m_dspare2, m_dspare, &hs1);
            inverse (m_dspare2, 0, cepOut);
        }
        
        void inverseCepstral (const float* magIn, float* cepOut) override
        {
            if (! m_fspec) initFloat();
            const int hs1 = m_size/2 + 1;
            FVO::copy (m_fspare, magIn, hs1);
            FVO::add (m_fspare, 0.000001, hs1);
            // vvlogf (m_fspare2, m_fspare, &hs1);
            inverse (m_fspare2, 0, cepOut);
        }
        
        
    private:
        
        void packReal (const float* const re)  // Pack input for forward transform
        {
            vDSP_ctoz ((DSPComplex *)re, 2, m_fpacked, 1, m_size/2);
        }
        
        void packReal (const double* const re)
        {
            vDSP_ctozD ((DSPDoubleComplex *)re, 2, m_dpacked, 1, m_size/2);
        }
        
        void packComplex (const float* const re, const float* const im)  // Pack input for inverse transform
        {
            const int num = m_size/2 + 1;
            
            if (re)
                FVO::copy (m_fpacked->realp, re, num);
            else
                FVO::fill (m_fpacked->realp, 0.0f, num);
            
            if (im)
                FVO::copy (m_fpacked->imagp, im, num);
            else
                FVO::fill (m_fpacked->imagp, 0.0f, num);
                           
            fnyq();
        }
                           
        void packComplex (const double* const re, const double* const im)
        {
            const int num = m_size/2 + 1;
           
           if (re)
               FVO::copy (m_dpacked->realp, re, num);
           else
               FVO::fill (m_dpacked->realp, 0.0, num);
           
           if (im)
               FVO::copy (m_dpacked->imagp, im, num);
           else
               FVO::fill (m_dpacked->imagp, 0.0, num);
           
           dnyq();
        }
        
        void unpackReal (float* const re)  // Unpack output for inverse transform
        {
            vDSP_ztoc (m_fpacked, 1, (DSPComplex *)re, 2, m_size/2);
        }
        
        void unpackReal (double* const re)
        {
            vDSP_ztocD (m_dpacked, 1, (DSPDoubleComplex *)re, 2, m_size/2);
        }
        
        void unpackComplex (float* const re, float* const im) // Unpack output for forward transform
        {
            const auto num = m_size/2 + 1;
            
            // vDSP forward FFTs are scaled 2x (for some reason)
            float two = 2.f;
            vDSP_vsdiv (m_fpacked->realp, 1, &two, re, 1, num);
            vDSP_vsdiv (m_fpacked->imagp, 1, &two, im, 1, num);
        }
        
        void unpackComplex (float* const cplx)
        {
            // vDSP forward FFTs are scaled 2x (for some reason)
            const int num = m_size/2 + 1;
            for (int i = 0; i < num; ++i)
            {
                const auto idx = i * 2;
                cplx[idx]   = m_fpacked->realp[i] * 0.5f;
                cplx[idx+1] = m_fpacked->imagp[i] * 0.5f;
            }
        }
        
        void unpackComplex (double* const re, double* const im)
        {
            const auto num = m_size/2 + 1;
            
            // vDSP forward FFTs are scaled 2x (for some reason)
            double two = 2.0;
            vDSP_vsdivD (m_dpacked->realp, 1, &two, re, 1, num);
            vDSP_vsdivD (m_dpacked->imagp, 1, &two, im, 1, num);
        }
        
        void unpackComplex(double* const cplx)
        {
            // vDSP forward FFTs are scaled 2x (for some reason)
            const int num = m_size/2 + 1;
            for (int i = 0; i < num; ++i)
            {
                const auto idx = i * 2;
                cplx[idx]   = m_dpacked->realp[i] * 0.5;
                cplx[idx+1] = m_dpacked->imagp[i] * 0.5;
            }
        }
        
        void fdenyq()  // for fft result in packed form, unpack the DC and Nyquist bins
        {
            const int hs = m_size/2;
            m_fpacked->realp[hs] = m_fpacked->imagp[0];
            m_fpacked->imagp[hs] = 0.f;
            m_fpacked->imagp[0] = 0.f;
        }
        
        void ddenyq()  // for fft result in packed form, unpack the DC and Nyquist bins
        {
            const int hs = m_size/2;
            m_dpacked->realp[hs] = m_dpacked->imagp[0];
            m_dpacked->imagp[hs] = 0.;
            m_dpacked->imagp[0] = 0.;
        }
        
        void fnyq()  // for ifft input in packed form, pack the DC and Nyquist bins
        {
            const int hs = m_size/2;
            m_fpacked->imagp[0] = m_fpacked->realp[hs];
            m_fpacked->realp[hs] = 0.f;
            m_fpacked->imagp[hs] = 0.f;
        }
        
        void dnyq()  // for ifft input in packed form, pack the DC and Nyquist bins
        {
            const int hs = m_size/2;
            m_dpacked->imagp[0] = m_dpacked->realp[hs];
            m_dpacked->realp[hs] = 0.;
            m_dpacked->imagp[hs] = 0.;
        }
        
        /*
        */
        
        const int m_size;
        int m_order;
        FFTSetup m_fspec;
        FFTSetupD m_dspec;
        DSPSplitComplex* m_fbuf;
        DSPDoubleSplitComplex* m_dbuf;
        DSPSplitComplex* m_fpacked;
        float* m_fspare;
        float* m_fspare2;
        DSPDoubleSplitComplex* m_dpacked;
        double* m_dspare;
        double* m_dspare2;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FFT)
    };
   
    
#elif BV_USE_IPP  /* if BV_USE_VDSP  */
    
    
#endif /* if BV_USE_VDSP  */
    
    
}  // namespace
