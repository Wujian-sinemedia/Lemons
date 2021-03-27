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
    
    
    
    /*
     */
    
#if BV_USE_VDSP
    
    class FFT :     public FFTinterface
    {
        using FVO = juce::FloatVectorOperations;
        
    public:
        FFT(int size) :
            m_size(size), m_fspec(nullptr), m_dspec(nullptr),
            m_fpacked(nullptr), m_fspare(nullptr),
            m_dpacked(nullptr), m_dspare(nullptr)
        {
            for (int i = 0; ; ++i) {
                if (m_size & (1 << i)) {
                    m_order = vDSP_Length(i);
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
            if (m_fspec != nullptr)
                return;
            
            m_fspec = vDSP_create_fftsetup (m_order, FFT_RADIX2);
            m_fbuf = new DSPSplitComplex;
            //!!! "If possible, tempBuffer->realp and tempBuffer->imagp should be 32-byte aligned for best performance."
            m_fbuf->realp = aligned_allocate<float>(size_t(m_size));
            m_fbuf->imagp = aligned_allocate<float>(size_t(m_size));
            m_fpacked = new DSPSplitComplex;
            m_fpacked->realp = aligned_allocate<float>(size_t(m_size / 2 + 1));
            m_fpacked->imagp = aligned_allocate<float>(size_t(m_size / 2 + 1));
            m_fspare = aligned_allocate<float>(size_t(m_size + 2));
            m_fspare2 = aligned_allocate<float>(size_t(m_size + 2));
        }
        
        void initDouble() override
        {
            if (m_dspec != nullptr)
                return;
            
            m_dspec = vDSP_create_fftsetupD (m_order, FFT_RADIX2);
            m_dbuf = new DSPDoubleSplitComplex;
            //!!! "If possible, tempBuffer->realp and tempBuffer->imagp should be 32-byte aligned for best performance."
            m_dbuf->realp = aligned_allocate<double>(size_t(m_size));
            m_dbuf->imagp = aligned_allocate<double>(size_t(m_size));
            m_dpacked = new DSPDoubleSplitComplex;
            m_dpacked->realp = aligned_allocate<double>(size_t(m_size / 2 + 1));
            m_dpacked->imagp = aligned_allocate<double>(size_t(m_size / 2 + 1));
            m_dspare = aligned_allocate<double>(size_t(m_size + 2));
            m_dspare2 = aligned_allocate<double>(size_t(m_size + 2));
        }
        
        
        bool isFloatInitialized() override
        {
            return m_fspec != nullptr;
        }
        
        bool isDoubleInitialized() override
        {
            return m_dspec != nullptr;
        }
        
        
        void forward (const double* BV_R_ realIn, double* BV_R_ realOut, double* BV_R_ imagOut) override
        {
            if (m_dspec == nullptr) initDouble();
            packReal (realIn);
            vDSP_fft_zriptD (m_dspec, m_dpacked, 1, m_dbuf, m_order, FFT_FORWARD);
            ddenyq();
            unpackComplex (realOut, imagOut);
        }
        
        void forward (const float* BV_R_ realIn, float* BV_R_ realOut, float* BV_R_ imagOut) override
        {
            if (m_fspec == nullptr) initFloat();
            packReal (realIn);
            vDSP_fft_zript (m_fspec, m_fpacked, 1, m_fbuf, m_order, FFT_FORWARD);
            fdenyq();
            unpackComplex (realOut, imagOut);
        }
        
        void forwardInterleaved (const double* BV_R_ realIn, double* BV_R_ complexOut) override
        {
            if (m_dspec == nullptr) initDouble();
            packReal (realIn);
            vDSP_fft_zriptD (m_dspec, m_dpacked, 1, m_dbuf, m_order, FFT_FORWARD);
            ddenyq();
            unpackComplex (complexOut);
        }
        
        void forwardInterleaved (const float* BV_R_ realIn, float* BV_R_ complexOut) override
        {
            if (m_fspec == nullptr) initFloat();
            packReal(realIn);
            vDSP_fft_zript (m_fspec, m_fpacked, 1, m_fbuf, m_order, FFT_FORWARD);
            fdenyq();
            unpackComplex (complexOut);
        }
        
        void forwardMagnitude (const double* BV_R_ realIn, double* BV_R_ magOut) override
        {
            if (m_dspec == nullptr) initDouble();
            packReal (realIn);
            vDSP_fft_zriptD (m_dspec, m_dpacked, 1, m_dbuf, m_order, FFT_FORWARD);
            ddenyq();
            const int hs1 = m_size/2+1;
            vDSP_zvmagsD (m_dpacked, 1, m_dspare, 1, vDSP_Length(hs1));
            vvsqrt (m_dspare2, m_dspare, &hs1);
            // vDSP forward FFTs are scaled 2x (for some reason)
            double two = 2.0;
            vDSP_vsdivD (m_dspare2, 1, &two, magOut, 1, vDSP_Length(hs1));
        }
        
        void forwardMagnitude (const float* BV_R_ realIn, float* BV_R_ magOut) override
        {
            if (m_fspec == nullptr) initFloat();
            packReal (realIn);
            vDSP_fft_zript (m_fspec, m_fpacked, 1, m_fbuf, m_order, FFT_FORWARD);
            fdenyq();
            const int hs1 = m_size/2 + 1;
            vDSP_zvmags (m_fpacked, 1, m_fspare, 1, vDSP_Length(hs1));
            vvsqrtf (m_fspare2, m_fspare, &hs1);
            // vDSP forward FFTs are scaled 2x (for some reason)
            float two = 2.f;
            vDSP_vsdiv (m_fspare2, 1, &two, magOut, 1, vDSP_Length(hs1));
        }
        
        void forwardPolar (const double* BV_R_ realIn, double* BV_R_ magOut, double* BV_R_ phaseOut) override
        {
            if (m_dspec == nullptr) initDouble();
            const int hs1 = m_size/2+1;
            packReal (realIn);
            vDSP_fft_zriptD (m_dspec, m_dpacked, 1, m_dbuf, m_order, FFT_FORWARD);
            ddenyq();
            // vDSP forward FFTs are scaled 2x (for some reason)
            for (int i = 0; i < hs1; ++i) m_dpacked->realp[i] *= 0.5;
            for (int i = 0; i < hs1; ++i) m_dpacked->imagp[i] *= 0.5;
            vecops::cartesian_to_polar (magOut, phaseOut,
                                        m_dpacked->realp, m_dpacked->imagp, hs1);
        }
        
        void forwardPolar (const float* BV_R_ realIn, float* BV_R_ magOut, float* BV_R_ phaseOut) override
        {
            if (m_fspec == nullptr) initFloat();
            const int hs1 = m_size/2+1;
            packReal (realIn);
            vDSP_fft_zript (m_fspec, m_fpacked, 1, m_fbuf, m_order, FFT_FORWARD);
            fdenyq();
            // vDSP forward FFTs are scaled 2x (for some reason)
            for (int i = 0; i < hs1; ++i) m_fpacked->realp[i] *= 0.5f;
            for (int i = 0; i < hs1; ++i) m_fpacked->imagp[i] *= 0.5f;
            vecops::cartesian_to_polar (magOut, phaseOut,
                                        m_fpacked->realp, m_fpacked->imagp, hs1);
        }
        
        /*
         */
        
        void inverse (const double* BV_R_ realIn, const double* BV_R_ imagIn, double* BV_R_ realOut) override
        {
            if (m_dspec == nullptr) initDouble();
            packComplex (realIn, imagIn);
            vDSP_fft_zriptD (m_dspec, m_dpacked, 1, m_dbuf, m_order, FFT_INVERSE);
            unpackReal (realOut);
        }
        
        void inverse (const float* BV_R_ realIn, const float* BV_R_ imagIn, float* BV_R_ realOut) override
        {
            if (m_fspec == nullptr) initFloat();
            packComplex (realIn, imagIn);
            vDSP_fft_zript (m_fspec, m_fpacked, 1, m_fbuf, m_order, FFT_INVERSE);
            unpackReal (realOut);
        }
        
        void inverseInterleaved (const double* BV_R_ complexIn, double* BV_R_ realOut) override
        {
            if (m_dspec == nullptr) initDouble();
            //            double *d[2] = { m_dpacked->realp, m_dpacked->imagp };
            //            vecops::deinterleave (d, complexIn, 2, m_size/2 + 1);
            vDSP_fft_zriptD (m_dspec, m_dpacked, 1, m_dbuf, m_order, FFT_INVERSE);
            unpackReal (realOut);
        }
        
        void inverseInterleaved (const float* BV_R_ complexIn, float* BV_R_ realOut) override
        {
            if (m_fspec == nullptr) initFloat();
            //            float *f[2] = { m_fpacked->realp, m_fpacked->imagp };
            //            vecops::deinterleave (f, complexIn, 2, m_size/2 + 1);
            vDSP_fft_zript (m_fspec, m_fpacked, 1, m_fbuf, m_order, FFT_INVERSE);
            unpackReal (realOut);
        }
        
        void inversePolar (const double* BV_R_ magIn, const double* BV_R_ phaseIn, double* BV_R_ realOut) override
        {
            if (m_dspec == nullptr) initDouble();
            const int hs1 = m_size/2+1;
            vvsincos (m_dpacked->imagp, m_dpacked->realp, phaseIn, &hs1);
            double *const rp = m_dpacked->realp;
            double *const ip = m_dpacked->imagp;
            for (int i = 0; i < hs1; ++i) rp[i] *= magIn[i];
            for (int i = 0; i < hs1; ++i) ip[i] *= magIn[i];
            dnyq();
            vDSP_fft_zriptD (m_dspec, m_dpacked, 1, m_dbuf, m_order, FFT_INVERSE);
            unpackReal (realOut);
        }
        
        void inversePolar (const float* BV_R_ magIn, const float* BV_R_ phaseIn, float* BV_R_ realOut) override
        {
            if (m_fspec == nullptr) initFloat();
            const int hs1 = m_size/2+1;
            vvsincosf (m_fpacked->imagp, m_fpacked->realp, phaseIn, &hs1);
            float *const rp = m_fpacked->realp;
            float *const ip = m_fpacked->imagp;
            for (int i = 0; i < hs1; ++i) rp[i] *= magIn[i];
            for (int i = 0; i < hs1; ++i) ip[i] *= magIn[i];
            fnyq();
            vDSP_fft_zript (m_fspec, m_fpacked, 1, m_fbuf, m_order, FFT_INVERSE);
            unpackReal (realOut);
        }
        
        void inverseCepstral (const double* BV_R_ magIn, double* BV_R_ cepOut) override
        {
            if (m_dspec == nullptr) initDouble();
            const int hs1 = m_size/2 + 1;
            FVO::copy (m_dspare, magIn, hs1);
            FVO::add (m_dspare, 0.000001, hs1);
            vvlog (m_dspare2, m_dspare, &hs1);
            inverse (m_dspare2, nullptr, cepOut);
        }
        
        void inverseCepstral (const float* BV_R_ magIn, float* BV_R_ cepOut) override
        {
            if (m_fspec == nullptr) initFloat();
            const int hs1 = m_size/2 + 1;
            FVO::copy (m_fspare, magIn, hs1);
            FVO::add (m_fspare, 0.000001f, hs1);
            vvlogf (m_fspare2, m_fspare, &hs1);
            inverse (m_fspare2, nullptr, cepOut);
        }
        
        
    private:
        
        BV_FORCE_INLINE void packReal (const float* const re)  // Pack input for forward transform
        {
            vDSP_ctoz ((DSPComplex *)re, 2, m_fpacked, 1, vDSP_Length(m_size/2));
        }
        
        BV_FORCE_INLINE void packReal (const double* const re)
        {
            vDSP_ctozD ((DSPDoubleComplex *)re, 2, m_dpacked, 1, vDSP_Length(m_size/2));
        }
        
        BV_FORCE_INLINE void packComplex (const float* const re, const float* const im)  // Pack input for inverse transform
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
        
        BV_FORCE_INLINE void packComplex (const double* const re, const double* const im)
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
        
        BV_FORCE_INLINE void unpackReal (float* const re)  // Unpack output for inverse transform
        {
            vDSP_ztoc (m_fpacked, 1, (DSPComplex *)re, 2, vDSP_Length(m_size/2));
        }
        
        BV_FORCE_INLINE void unpackReal (double* const re)
        {
            vDSP_ztocD (m_dpacked, 1, (DSPDoubleComplex *)re, 2, vDSP_Length(m_size/2));
        }
        
        BV_FORCE_INLINE void unpackComplex (float* const re, float* const im) // Unpack output for forward transform
        {
            const auto num = vDSP_Length(m_size/2 + 1);
            
            // vDSP forward FFTs are scaled 2x (for some reason)
            float two = 2.f;
            vDSP_vsdiv (m_fpacked->realp, 1, &two, re, 1, num);
            vDSP_vsdiv (m_fpacked->imagp, 1, &two, im, 1, num);
        }
        
        BV_FORCE_INLINE void unpackComplex (float* const cplx)
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
        
        BV_FORCE_INLINE void unpackComplex (double* const re, double* const im)
        {
            const auto num = vDSP_Length(m_size/2 + 1);
            
            // vDSP forward FFTs are scaled 2x (for some reason)
            double two = 2.0;
            vDSP_vsdivD (m_dpacked->realp, 1, &two, re, 1, num);
            vDSP_vsdivD (m_dpacked->imagp, 1, &two, im, 1, num);
        }
        
        BV_FORCE_INLINE void unpackComplex (double* const cplx)
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
        
        BV_FORCE_INLINE void fdenyq()  // for fft result in packed form, unpack the DC and Nyquist bins
        {
            const int hs = m_size/2;
            m_fpacked->realp[hs] = m_fpacked->imagp[0];
            m_fpacked->imagp[hs] = 0.f;
            m_fpacked->imagp[0] = 0.f;
        }
        
        BV_FORCE_INLINE void ddenyq()  // for fft result in packed form, unpack the DC and Nyquist bins
        {
            const int hs = m_size/2;
            m_dpacked->realp[hs] = m_dpacked->imagp[0];
            m_dpacked->imagp[hs] = 0.;
            m_dpacked->imagp[0] = 0.;
        }
        
        BV_FORCE_INLINE void fnyq()  // for ifft input in packed form, pack the DC and Nyquist bins
        {
            const int hs = m_size/2;
            m_fpacked->imagp[0] = m_fpacked->realp[hs];
            m_fpacked->realp[hs] = 0.f;
            m_fpacked->imagp[hs] = 0.f;
        }
        
        BV_FORCE_INLINE void dnyq()  // for ifft input in packed form, pack the DC and Nyquist bins
        {
            const int hs = m_size/2;
            m_dpacked->imagp[0] = m_dpacked->realp[hs];
            m_dpacked->realp[hs] = 0.;
            m_dpacked->imagp[hs] = 0.;
        }
        
        /*
         */
        
        const int m_size;
        vDSP_Length m_order;
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
    
    
    class FFT :     public FFTinterface
    {
    public:
        FFT(int size) :
            m_size(size), m_fspec(0), m_dspec(0)
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
            if (m_fspec != nullptr)
            {
#if (IPP_VERSION_MAJOR >= 9)
                ippsFree (m_fspecbuf);
#else
                ippsFFTFree_R_32f (m_fspec);
#endif
                ippsFree (m_fbuf);
                ippsFree (m_fpacked);
                ippsFree (m_fspare);
            }
            
            if (m_dspec != nullptr)
            {
#if (IPP_VERSION_MAJOR >= 9)
                ippsFree (m_dspecbuf);
#else
                ippsFFTFree_R_64f (m_dspec);
#endif
                ippsFree (m_dbuf);
                ippsFree (m_dpacked);
                ippsFree (m_dspare);
            }
        }
        
        int getSize() const override
        {
            return m_size;
        }
        
        
        void initFloat() override
        {
            if (m_fspec != nullptr)
                return;
            
#if (IPP_VERSION_MAJOR >= 9)
            int specSize, specBufferSize, bufferSize;
            ippsFFTGetSize_R_32f (m_order, IPP_FFT_NODIV_BY_ANY, ippAlgHintFast,
                                  &specSize, &specBufferSize, &bufferSize);
            m_fspecbuf = ippsMalloc_8u (specSize);
            Ipp8u *tmp = ippsMalloc_8u (specBufferSize);
            m_fbuf = ippsMalloc_8u (bufferSize);
            m_fpacked = ippsMalloc_32f (m_size + 2);
            m_fspare = ippsMalloc_32f (m_size / 2 + 1);
            ippsFFTInit_R_32f (&m_fspec,
                               m_order, IPP_FFT_NODIV_BY_ANY, ippAlgHintFast,
                               m_fspecbuf, tmp);
            ippsFree(tmp);
#else
            int specSize, specBufferSize, bufferSize;
            ippsFFTGetSize_R_32f (m_order, IPP_FFT_NODIV_BY_ANY, ippAlgHintFast,
                                  &specSize, &specBufferSize, &bufferSize);
            m_fbuf = ippsMalloc_8u (bufferSize);
            m_fpacked = ippsMalloc_32f (m_size + 2);
            m_fspare = ippsMalloc_32f (m_size / 2 + 1);
            ippsFFTInitAlloc_R_32f (&m_fspec, m_order, IPP_FFT_NODIV_BY_ANY,
                                    ippAlgHintFast);
#endif
        }
        
        void initDouble() override
        {
            if (m_dspec 1= nullptr)
                return;
            
#if (IPP_VERSION_MAJOR >= 9)
            int specSize, specBufferSize, bufferSize;
            ippsFFTGetSize_R_64f (m_order, IPP_FFT_NODIV_BY_ANY, ippAlgHintFast,
                                  &specSize, &specBufferSize, &bufferSize);
            m_dspecbuf = ippsMalloc_8u (specSize);
            Ipp8u *tmp = ippsMalloc_8u (specBufferSize);
            m_dbuf = ippsMalloc_8u (bufferSize);
            m_dpacked = ippsMalloc_64f (m_size + 2);
            m_dspare = ippsMalloc_64f (m_size / 2 + 1);
            ippsFFTInit_R_64f (&m_dspec,
                               m_order, IPP_FFT_NODIV_BY_ANY, ippAlgHintFast,
                               m_dspecbuf, tmp);
            ippsFree(tmp);
#else
            int specSize, specBufferSize, bufferSize;
            ippsFFTGetSize_R_64f (m_order, IPP_FFT_NODIV_BY_ANY, ippAlgHintFast,
                                  &specSize, &specBufferSize, &bufferSize);
            m_dbuf = ippsMalloc_8u (bufferSize);
            m_dpacked = ippsMalloc_64f (m_size + 2);
            m_dspare = ippsMalloc_64f (m_size / 2 + 1);
            ippsFFTInitAlloc_R_64f (&m_dspec, m_order, IPP_FFT_NODIV_BY_ANY,
                                    ippAlgHintFast);
#endif
        }
        
        
        bool isFloatInitialized() override
        {
            return m_fspec != nullptr;
        }
        
        bool isDoubleInitialized() override
        {
            return m_dspec != nullptr;
        }
        
        
        void forward (const double* BV_R_ realIn, double* BV_R_ realOut, double* BV_R_ imagOut) override
        {
            if (m_dspec == nullptr) initDouble();
            ippsFFTFwd_RToCCS_64f (realIn, m_dpacked, m_dspec, m_dbuf);
            unpackDouble (realOut, imagOut);
        }
        
        void forward (const float* BV_R_ realIn, float* BV_R_ realOut, float* BV_R_ imagOut) override
        {
            if (m_fspec == nullptr) initFloat();
            ippsFFTFwd_RToCCS_32f (realIn, m_fpacked, m_fspec, m_fbuf);
            unpackFloat (realOut, imagOut);
        }
        
        void forwardInterleaved (const double* BV_R_ realIn, double* BV_R_ complexOut) override
        {
            if (m_dspec == nullptr) initDouble();
            ippsFFTFwd_RToCCS_64f (realIn, complexOut, m_dspec, m_dbuf);
        }
        
        void forwardInterleaved (const float* BV_R_ realIn, float* BV_R_ complexOut) override
        {
            if (m_fspec == nullptr) initFloat();
            ippsFFTFwd_RToCCS_32f (realIn, complexOut, m_fspec, m_fbuf);
        }
    
        void forwardPolar (const double* BV_R_ realIn, double* BV_R_ magOut, double* BV_R_ phaseOut) override
        {
            if (m_dspec == nullptr) initDouble();
            ippsFFTFwd_RToCCS_64f (realIn, m_dpacked, m_dspec, m_dbuf);
            unpackDouble (m_dpacked, m_dspare);
            ippsCartToPolar_64f (m_dpacked, m_dspare, magOut, phaseOut, m_size/2+1);
        }
        
        void forwardPolar (const float* BV_R_ realIn, float* BV_R_ magOut, float* BV_R_ phaseOut) override
        {
            if (m_fspec == nullptr) initFloat();
            ippsFFTFwd_RToCCS_32f (realIn, m_fpacked, m_fspec, m_fbuf);
            unpackFloat (m_fpacked, m_fspare);
            ippsCartToPolar_32f (m_fpacked, m_fspare, magOut, phaseOut, m_size/2+1);
        }
        
        void forwardMagnitude (const double* BV_R_ realIn, double* BV_R_ magOut) override
        {
            if (m_dspec == nullptr) initDouble();
            ippsFFTFwd_RToCCS_64f (realIn, m_dpacked, m_dspec, m_dbuf);
            unpackDouble (m_dpacked, m_dspare);
            ippsMagnitude_64f (m_dpacked, m_dspare, magOut, m_size/2+1);
        }
        
        void forwardMagnitude (const float* BV_R_ realIn, float* BV_R_ magOut) override
        {
            if (m_fspec == nullptr) initFloat();
            ippsFFTFwd_RToCCS_32f (realIn, m_fpacked, m_fspec, m_fbuf);
            unpackFloat (m_fpacked, m_fspare);
            ippsMagnitude_32f (m_fpacked, m_fspare, magOut, m_size/2+1);
        }
        
        /*
        */
        
        void inverse (const double* BV_R_ realIn, const double* BV_R_ imagIn, double* BV_R_ realOut) override
        {
            if (m_dspec == nullptr) initDouble();
            packDouble (realIn, imagIn);
            ippsFFTInv_CCSToR_64f (m_dpacked, realOut, m_dspec, m_dbuf);
        }
        
        void inverse (const float* BV_R_ realIn, const float* BV_R_ imagIn, float* BV_R_ realOut) override
        {
            if (m_fspec == nullptr) initFloat();
            packFloat (realIn, imagIn);
            ippsFFTInv_CCSToR_32f (m_fpacked, realOut, m_fspec, m_fbuf);
        }
        
        void inverseInterleaved (const double* BV_R_ complexIn, double* BV_R_ realOut) override
        {
            if (m_dspec == nullptr) initDouble();
            ippsFFTInv_CCSToR_64f (complexIn, realOut, m_dspec, m_dbuf);
        }
        
        void inverseInterleaved (const float* BV_R_ complexIn, float* BV_R_ realOut) override
        {
            if (m_fspec == nullptr) initFloat();
            ippsFFTInv_CCSToR_32f (complexIn, realOut, m_fspec, m_fbuf);
        }
        
        void inversePolar (const double* BV_R_ magIn, const double* BV_R_ phaseIn, double* BV_R_ realOut) override
        {
            if (m_dspec == nullptr) initDouble();
            ippsPolarToCart_64f (magIn, phaseIn, realOut, m_dspare, m_size/2+1);
            packDouble (realOut, m_dspare); // to m_dpacked
            ippsFFTInv_CCSToR_64f (m_dpacked, realOut, m_dspec, m_dbuf);
        }
        
        void inversePolar (const float* BV_R_ magIn, const float* BV_R_ phaseIn, float* BV_R_ realOut) override
        {
            if (m_fspec == nullptr) initFloat();
            ippsPolarToCart_32f (magIn, phaseIn, realOut, m_fspare, m_size/2+1);
            packFloat (realOut, m_fspare); // to m_fpacked
            ippsFFTInv_CCSToR_32f (m_fpacked, realOut, m_fspec, m_fbuf);
        }
        
        void inverseCepstral (const double* BV_R_ magIn, double* BV_R_ cepOut) override
        {
            if (m_dspec == nullptr) initDouble();
            const int hs1 = m_size/2 + 1;
            ippsCopy_64f (magIn, m_dspare, hs1);
            ippsAddC_64f_I (0.000001, m_dspare, hs1);
            ippsLn_64f_I (m_dspare, hs1);
            packDouble (m_dspare, 0);
            ippsFFTInv_CCSToR_64f (m_dpacked, cepOut, m_dspec, m_dbuf);
        }
        
        void inverseCepstral (const float* BV_R_ magIn, float* BV_R_ cepOut) override
        {
            if (m_fspec == nullptr) initFloat();
            const int hs1 = m_size/2 + 1;
            ippsCopy_32f (magIn, m_fspare, hs1);
            ippsAddC_32f_I (0.000001f, m_fspare, hs1);
            ippsLn_32f_I (m_fspare, hs1);
            packFloat (m_fspare, 0);
            ippsFFTInv_CCSToR_32f (m_fpacked, cepOut, m_fspec, m_fbuf);
        }
        
        
    private:
        BV_FORCE_INLINE void packFloat (const float* BV_R_ re, const float* BV_R_ im)
        {
            int index = 0;
            const int hs = m_size/2;
            for (int i = 0; i <= hs; ++i) {
                m_fpacked[index++] = re[i];
                index++;
            }
            index = 0;
            if (im != nullptr) {
                for (int i = 0; i <= hs; ++i) {
                    index++;
                    m_fpacked[index++] = im[i];
                }
            } else {
                for (int i = 0; i <= hs; ++i) {
                    index++;
                    m_fpacked[index++] = 0.f;
                }
            }
        }
        
        BV_FORCE_INLINE void packDouble (const double* BV_R_ re, const double* BV_R_ im)
        {
            int index = 0;
            const int hs = m_size/2;
            for (int i = 0; i <= hs; ++i) {
                m_dpacked[index++] = re[i];
                index++;
            }
            index = 0;
            if (im != nullptr) {
                for (int i = 0; i <= hs; ++i) {
                    index++;
                    m_dpacked[index++] = im[i];
                }
            } else {
                for (int i = 0; i <= hs; ++i) {
                    index++;
                    m_dpacked[index++] = 0.0;
                }
            }
        }
        
        BV_FORCE_INLINE void unpackFloat (float* re, float* BV_R_ im)  // re may be equal to m_fpacked
        {
            int index = 0;
            const int hs = m_size/2;
            if (im != nullptr) {
                for (int i = 0; i <= hs; ++i) {
                    index++;
                    im[i] = m_fpacked[index++];
                }
            }
            index = 0;
            for (int i = 0; i <= hs; ++i) {
                re[i] = m_fpacked[index++];
                index++;
            }
        }
        
        BV_FORCE_INLINE void unpackDouble (double* re, double* BV_R_ im)  // re may be equal to m_dpacked
        {
            int index = 0;
            const int hs = m_size/2;
            
            if (im != nullptr) {
                for (int i = 0; i <= hs; ++i) {
                    index++;
                    im[i] = m_dpacked[index++];
                }
            }
            index = 0;
            for (int i = 0; i <= hs; ++i) {
                re[i] = m_dpacked[index++];
                index++;
            }
        }
        
        /*
        */
        
        const int m_size;
        int m_order;
        IppsFFTSpec_R_32f* m_fspec;
        IppsFFTSpec_R_64f* m_dspec;
        Ipp8u* m_fspecbuf;
        Ipp8u* m_dspecbuf;
        Ipp8u* m_fbuf;
        Ipp8u* m_dbuf;
        float* m_fpacked;
        float* m_fspare;
        double* m_dpacked;
        double* m_dspare;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FFT)
    };
    
#else  /* elif BV_USE_IPP  */
    
    
    
#endif /* if BV_USE_VDSP  */
    
    
}  // namespace

