
/*
FFT class using Apple's vDSP.
*/


namespace bav::dsp
{
/* vDSP only supports power-of-two FFT sizes */

class FFT : public FFTinterface
{
public:
    FFT (int size)
        : m_size (size), m_fspec (nullptr), m_dspec (nullptr), m_fpacked (nullptr), m_fspare (nullptr), m_dpacked (nullptr), m_dspare (nullptr)
    {
        jassert (
            math::isPowerOfTwo (size));  // vDSP only supports power-of-two FFT sizes
        for (int i = 0;; ++i)
        {
            if (m_size & (1 << i))
            {
                m_order = vDSP_Length (i);
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


    int getSize() const override { return m_size; }


    void initFloat() override
    {
        if (m_fspec != nullptr) return;

        m_fspec = vDSP_create_fftsetup (m_order, FFT_RADIX2);
        m_fbuf  = new DSPSplitComplex;
        //!!! "If possible, tempBuffer->realp and tempBuffer->imagp should be 32-byte aligned for best performance."
        m_fbuf->realp    = aligned_allocate< float > (size_t (m_size));
        m_fbuf->imagp    = aligned_allocate< float > (size_t (m_size));
        m_fpacked        = new DSPSplitComplex;
        m_fpacked->realp = aligned_allocate< float > (size_t (m_size / 2 + 1));
        m_fpacked->imagp = aligned_allocate< float > (size_t (m_size / 2 + 1));
        m_fspare         = aligned_allocate< float > (size_t (m_size + 2));
        m_fspare2        = aligned_allocate< float > (size_t (m_size + 2));
    }

    void initDouble() override
    {
        if (m_dspec != nullptr) return;

        m_dspec = vDSP_create_fftsetupD (m_order, FFT_RADIX2);
        m_dbuf  = new DSPDoubleSplitComplex;
        //!!! "If possible, tempBuffer->realp and tempBuffer->imagp should be 32-byte aligned for best performance."
        m_dbuf->realp    = aligned_allocate< double > (size_t (m_size));
        m_dbuf->imagp    = aligned_allocate< double > (size_t (m_size));
        m_dpacked        = new DSPDoubleSplitComplex;
        m_dpacked->realp = aligned_allocate< double > (size_t (m_size / 2 + 1));
        m_dpacked->imagp = aligned_allocate< double > (size_t (m_size / 2 + 1));
        m_dspare         = aligned_allocate< double > (size_t (m_size + 2));
        m_dspare2        = aligned_allocate< double > (size_t (m_size + 2));
    }


    bool isFloatInitialized() override { return m_fspec != nullptr; }

    bool isDoubleInitialized() override { return m_dspec != nullptr; }


    void forward (const double* BV_R_ realIn,
                  double* BV_R_       realOut,
                  double* BV_R_       imagOut) override
    {
        if (m_dspec == nullptr) initDouble();
        packReal (realIn);
        vDSP_fft_zriptD (m_dspec, m_dpacked, 1, m_dbuf, m_order, FFT_FORWARD);
        ddenyq();
        unpackComplex (realOut, imagOut);
    }

    void forward (const float* BV_R_ realIn,
                  float* BV_R_       realOut,
                  float* BV_R_       imagOut) override
    {
        if (m_fspec == nullptr) initFloat();
        packReal (realIn);
        vDSP_fft_zript (m_fspec, m_fpacked, 1, m_fbuf, m_order, FFT_FORWARD);
        fdenyq();
        unpackComplex (realOut, imagOut);
    }

    void forwardInterleaved (const double* BV_R_ realIn,
                             double* BV_R_       complexOut) override
    {
        if (m_dspec == nullptr) initDouble();
        packReal (realIn);
        vDSP_fft_zriptD (m_dspec, m_dpacked, 1, m_dbuf, m_order, FFT_FORWARD);
        ddenyq();
        unpackComplex (complexOut);
    }

    void forwardInterleaved (const float* BV_R_ realIn,
                             float* BV_R_       complexOut) override
    {
        if (m_fspec == nullptr) initFloat();
        packReal (realIn);
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
        const int hs1 = m_size / 2 + 1;
        vDSP_zvmagsD (m_dpacked, 1, m_dspare, 1, vDSP_Length (hs1));
        vvsqrt (m_dspare2, m_dspare, &hs1);
        // vDSP forward FFTs are scaled 2x (for some reason)
        double two = 2.0;
        vDSP_vsdivD (m_dspare2, 1, &two, magOut, 1, vDSP_Length (hs1));
    }

    void forwardMagnitude (const float* BV_R_ realIn, float* BV_R_ magOut) override
    {
        if (m_fspec == nullptr) initFloat();
        packReal (realIn);
        vDSP_fft_zript (m_fspec, m_fpacked, 1, m_fbuf, m_order, FFT_FORWARD);
        fdenyq();
        const int hs1 = m_size / 2 + 1;
        vDSP_zvmags (m_fpacked, 1, m_fspare, 1, vDSP_Length (hs1));
        vvsqrtf (m_fspare2, m_fspare, &hs1);
        // vDSP forward FFTs are scaled 2x (for some reason)
        float two = 2.f;
        vDSP_vsdiv (m_fspare2, 1, &two, magOut, 1, vDSP_Length (hs1));
    }

    void forwardPolar (const double* BV_R_ realIn,
                       double* BV_R_       magOut,
                       double* BV_R_       phaseOut) override
    {
        if (m_dspec == nullptr) initDouble();
        const int hs1 = m_size / 2 + 1;
        packReal (realIn);
        vDSP_fft_zriptD (m_dspec, m_dpacked, 1, m_dbuf, m_order, FFT_FORWARD);
        ddenyq();
        // vDSP forward FFTs are scaled 2x (for some reason)
        for (int i = 0; i < hs1; ++i)
            m_dpacked->realp[i] *= 0.5;
        for (int i = 0; i < hs1; ++i)
            m_dpacked->imagp[i] *= 0.5;
        vecops::cartesian_to_polar (
            magOut, phaseOut, m_dpacked->realp, m_dpacked->imagp, hs1);
    }

    void forwardPolar (const float* BV_R_ realIn,
                       float* BV_R_       magOut,
                       float* BV_R_       phaseOut) override
    {
        if (m_fspec == nullptr) initFloat();
        const int hs1 = m_size / 2 + 1;
        packReal (realIn);
        vDSP_fft_zript (m_fspec, m_fpacked, 1, m_fbuf, m_order, FFT_FORWARD);
        fdenyq();
        // vDSP forward FFTs are scaled 2x (for some reason)
        for (int i = 0; i < hs1; ++i)
            m_fpacked->realp[i] *= 0.5f;
        for (int i = 0; i < hs1; ++i)
            m_fpacked->imagp[i] *= 0.5f;
        vecops::cartesian_to_polar (
            magOut, phaseOut, m_fpacked->realp, m_fpacked->imagp, hs1);
    }

    /*
 */

    void inverse (const double* BV_R_ realIn,
                  const double* BV_R_ imagIn,
                  double* BV_R_       realOut) override
    {
        if (m_dspec == nullptr) initDouble();
        packComplex (realIn, imagIn);
        vDSP_fft_zriptD (m_dspec, m_dpacked, 1, m_dbuf, m_order, FFT_INVERSE);
        unpackReal (realOut);
    }

    void inverse (const float* BV_R_ realIn,
                  const float* BV_R_ imagIn,
                  float* BV_R_       realOut) override
    {
        if (m_fspec == nullptr) initFloat();
        packComplex (realIn, imagIn);
        vDSP_fft_zript (m_fspec, m_fpacked, 1, m_fbuf, m_order, FFT_INVERSE);
        unpackReal (realOut);
    }

    void inverseInterleaved (const double* BV_R_ complexIn,
                             double* BV_R_       realOut) override
    {
        if (m_dspec == nullptr) initDouble();
        double* d[2] = {m_dpacked->realp, m_dpacked->imagp};
        vecops::deinterleave (d, complexIn, 2, m_size / 2 + 1);
        vDSP_fft_zriptD (m_dspec, m_dpacked, 1, m_dbuf, m_order, FFT_INVERSE);
        unpackReal (realOut);
    }

    void inverseInterleaved (const float* BV_R_ complexIn,
                             float* BV_R_       realOut) override
    {
        if (m_fspec == nullptr) initFloat();
        float* f[2] = {m_fpacked->realp, m_fpacked->imagp};
        vecops::deinterleave (f, complexIn, 2, m_size / 2 + 1);
        vDSP_fft_zript (m_fspec, m_fpacked, 1, m_fbuf, m_order, FFT_INVERSE);
        unpackReal (realOut);
    }

    void inversePolar (const double* BV_R_ magIn,
                       const double* BV_R_ phaseIn,
                       double* BV_R_       realOut) override
    {
        if (m_dspec == nullptr) initDouble();
        const int hs1 = m_size / 2 + 1;
        vvsincos (m_dpacked->imagp, m_dpacked->realp, phaseIn, &hs1);
        double* const rp = m_dpacked->realp;
        double* const ip = m_dpacked->imagp;
        for (int i = 0; i < hs1; ++i)
            rp[i] *= magIn[i];
        for (int i = 0; i < hs1; ++i)
            ip[i] *= magIn[i];
        dnyq();
        vDSP_fft_zriptD (m_dspec, m_dpacked, 1, m_dbuf, m_order, FFT_INVERSE);
        unpackReal (realOut);
    }

    void inversePolar (const float* BV_R_ magIn,
                       const float* BV_R_ phaseIn,
                       float* BV_R_       realOut) override
    {
        if (m_fspec == nullptr) initFloat();
        const int hs1 = m_size / 2 + 1;
        vvsincosf (m_fpacked->imagp, m_fpacked->realp, phaseIn, &hs1);
        float* const rp = m_fpacked->realp;
        float* const ip = m_fpacked->imagp;
        for (int i = 0; i < hs1; ++i)
            rp[i] *= magIn[i];
        for (int i = 0; i < hs1; ++i)
            ip[i] *= magIn[i];
        fnyq();
        vDSP_fft_zript (m_fspec, m_fpacked, 1, m_fbuf, m_order, FFT_INVERSE);
        unpackReal (realOut);
    }

    void inverseCepstral (const double* BV_R_ magIn, double* BV_R_ cepOut) override
    {
        if (m_dspec == nullptr) initDouble();
        const int hs1 = m_size / 2 + 1;

        memcpy (m_dspare, magIn, (size_t) hs1 * sizeof (double));

        const double tinyvalue = 0.000001;
        vDSP_vsaddD (m_dspare,
                     vDSP_Stride (1),
                     &tinyvalue,
                     m_dspare,
                     vDSP_Stride (1),
                     vDSP_Length (hs1));

        vvlog (m_dspare2, m_dspare, &hs1);
        inverse (m_dspare2, nullptr, cepOut);
    }

    void inverseCepstral (const float* BV_R_ magIn, float* BV_R_ cepOut) override
    {
        if (m_fspec == nullptr) initFloat();
        const int hs1 = m_size / 2 + 1;

        memcpy (m_fspare, magIn, (size_t) hs1 * sizeof (float));

        const float tinyvalue = 0.000001f;
        vDSP_vsadd (m_fspare,
                    vDSP_Stride (1),
                    &tinyvalue,
                    m_fspare,
                    vDSP_Stride (1),
                    vDSP_Length (hs1));

        vvlogf (m_fspare2, m_fspare, &hs1);
        inverse (m_fspare2, nullptr, cepOut);
    }


private:
    BV_FORCE_INLINE void
        packReal (const float* const re)  // Pack input for forward transform
    {
        vDSP_ctoz ((DSPComplex*) re, 2, m_fpacked, 1, vDSP_Length (m_size / 2));
    }

    BV_FORCE_INLINE void packReal (const double* const re)
    {
        vDSP_ctozD (
            (DSPDoubleComplex*) re, 2, m_dpacked, 1, vDSP_Length (m_size / 2));
    }

    BV_FORCE_INLINE void
        packComplex (const float* const re,
                     const float* const im)  // Pack input for inverse transform
    {
        const int num = m_size / 2 + 1;

        const float zero = 0.0f;

        if (re == nullptr)
            vDSP_vfill (&zero, m_fpacked->realp, vDSP_Stride (1), vDSP_Length (num));
        else
            memcpy (m_fpacked->realp, re, (size_t) num * sizeof (float));

        if (im == nullptr)
            vDSP_vfill (&zero, m_fpacked->imagp, vDSP_Stride (1), vDSP_Length (num));
        else
            memcpy (m_fpacked->imagp, im, (size_t) num * sizeof (float));

        fnyq();
    }

    BV_FORCE_INLINE void packComplex (const double* const re, const double* const im)
    {
        const int num = m_size / 2 + 1;

        const double zero = 0.0;

        if (re == nullptr)
            vDSP_vfillD (
                &zero, m_dpacked->realp, vDSP_Stride (1), vDSP_Length (num));
        else
            memcpy (m_dpacked->realp, re, (size_t) num * sizeof (double));

        if (im == nullptr)
            vDSP_vfillD (
                &zero, m_dpacked->imagp, vDSP_Stride (1), vDSP_Length (num));
        else
            memcpy (m_dpacked->imagp, im, (size_t) num * sizeof (double));

        dnyq();
    }

    BV_FORCE_INLINE void
        unpackReal (float* const re)  // Unpack output for inverse transform
    {
        vDSP_ztoc (m_fpacked, 1, (DSPComplex*) re, 2, vDSP_Length (m_size / 2));
    }

    BV_FORCE_INLINE void unpackReal (double* const re)
    {
        vDSP_ztocD (
            m_dpacked, 1, (DSPDoubleComplex*) re, 2, vDSP_Length (m_size / 2));
    }

    BV_FORCE_INLINE void
        unpackComplex (float* const re,
                       float* const im)  // Unpack output for forward transform
    {
        const auto num = vDSP_Length (m_size / 2 + 1);

        // vDSP forward FFTs are scaled 2x (for some reason)
        float two = 2.f;
        vDSP_vsdiv (m_fpacked->realp, 1, &two, re, 1, num);
        vDSP_vsdiv (m_fpacked->imagp, 1, &two, im, 1, num);
    }

    BV_FORCE_INLINE void unpackComplex (float* const cplx)
    {
        // vDSP forward FFTs are scaled 2x (for some reason)
        const int num = m_size / 2 + 1;
        for (int i = 0; i < num; ++i)
        {
            const auto idx = i * 2;
            cplx[idx]      = m_fpacked->realp[i] * 0.5f;
            cplx[idx + 1]  = m_fpacked->imagp[i] * 0.5f;
        }
    }

    BV_FORCE_INLINE void unpackComplex (double* const re, double* const im)
    {
        const auto num = vDSP_Length (m_size / 2 + 1);

        // vDSP forward FFTs are scaled 2x (for some reason)
        double two = 2.0;
        vDSP_vsdivD (m_dpacked->realp, 1, &two, re, 1, num);
        vDSP_vsdivD (m_dpacked->imagp, 1, &two, im, 1, num);
    }

    BV_FORCE_INLINE void unpackComplex (double* const cplx)
    {
        // vDSP forward FFTs are scaled 2x (for some reason)
        const int num = m_size / 2 + 1;
        for (int i = 0; i < num; ++i)
        {
            const auto idx = i * 2;
            cplx[idx]      = m_dpacked->realp[i] * 0.5;
            cplx[idx + 1]  = m_dpacked->imagp[i] * 0.5;
        }
    }

    BV_FORCE_INLINE void
        fdenyq()  // for fft result in packed form, unpack the DC and Nyquist bins
    {
        const int hs         = m_size / 2;
        m_fpacked->realp[hs] = m_fpacked->imagp[0];
        m_fpacked->imagp[hs] = 0.f;
        m_fpacked->imagp[0]  = 0.f;
    }

    BV_FORCE_INLINE void
        ddenyq()  // for fft result in packed form, unpack the DC and Nyquist bins
    {
        const int hs         = m_size / 2;
        m_dpacked->realp[hs] = m_dpacked->imagp[0];
        m_dpacked->imagp[hs] = 0.;
        m_dpacked->imagp[0]  = 0.;
    }

    BV_FORCE_INLINE void
        fnyq()  // for ifft input in packed form, pack the DC and Nyquist bins
    {
        const int hs         = m_size / 2;
        m_fpacked->imagp[0]  = m_fpacked->realp[hs];
        m_fpacked->realp[hs] = 0.f;
        m_fpacked->imagp[hs] = 0.f;
    }

    BV_FORCE_INLINE void
        dnyq()  // for ifft input in packed form, pack the DC and Nyquist bins
    {
        const int hs         = m_size / 2;
        m_dpacked->imagp[0]  = m_dpacked->realp[hs];
        m_dpacked->realp[hs] = 0.;
        m_dpacked->imagp[hs] = 0.;
    }

    /*
 */

    const int              m_size;
    vDSP_Length            m_order;
    FFTSetup               m_fspec;
    FFTSetupD              m_dspec;
    DSPSplitComplex*       m_fbuf;
    DSPDoubleSplitComplex* m_dbuf;
    DSPSplitComplex*       m_fpacked;
    float*                 m_fspare;
    float*                 m_fspare2;
    DSPDoubleSplitComplex* m_dpacked;
    double*                m_dspare;
    double*                m_dspare2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FFT)
};


}  // namespace bav::dsp
