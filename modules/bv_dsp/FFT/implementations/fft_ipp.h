/*
FFT class using Intel IPP.
*/


namespace bav::dsp
{
/* IPP only supports power-of-two FFT sizes */

class FFT : public FFTinterface
{
public:
    FFT (int size)
        : m_size (size), m_fspec (0), m_dspec (0)
    {
        jassert (
            math::isPowerOfTwo (size));  // IPP only supports power-of-two FFT sizes
        for (int i = 0;; ++i)
        {
            if (m_size & (1 << i))
            {
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

    int getSize() const override { return m_size; }


    void initFloat() override
    {
        if (m_fspec != nullptr) return;

#if (IPP_VERSION_MAJOR >= 9)
        int specSize, specBufferSize, bufferSize;
        ippsFFTGetSize_R_32f (m_order,
                              IPP_FFT_NODIV_BY_ANY,
                              ippAlgHintFast,
                              &specSize,
                              &specBufferSize,
                              &bufferSize);
        m_fspecbuf = ippsMalloc_8u (specSize);
        Ipp8u* tmp = ippsMalloc_8u (specBufferSize);
        m_fbuf     = ippsMalloc_8u (bufferSize);
        m_fpacked  = ippsMalloc_32f (m_size + 2);
        m_fspare   = ippsMalloc_32f (m_size / 2 + 1);
        ippsFFTInit_R_32f (&m_fspec,
                           m_order,
                           IPP_FFT_NODIV_BY_ANY,
                           ippAlgHintFast,
                           m_fspecbuf,
                           tmp);
        ippsFree (tmp);
#else
        int specSize, specBufferSize, bufferSize;
        ippsFFTGetSize_R_32f (m_order,
                              IPP_FFT_NODIV_BY_ANY,
                              ippAlgHintFast,
                              &specSize,
                              &specBufferSize,
                              &bufferSize);
        m_fbuf = ippsMalloc_8u (bufferSize);
        m_fpacked = ippsMalloc_32f (m_size + 2);
        m_fspare = ippsMalloc_32f (m_size / 2 + 1);
        ippsFFTInitAlloc_R_32f (
            &m_fspec, m_order, IPP_FFT_NODIV_BY_ANY, ippAlgHintFast);
#endif
    }

    void initDouble() override
    {
        if (m_dspec 1 = nullptr) return;

#if (IPP_VERSION_MAJOR >= 9)
        int specSize, specBufferSize, bufferSize;
        ippsFFTGetSize_R_64f (m_order,
                              IPP_FFT_NODIV_BY_ANY,
                              ippAlgHintFast,
                              &specSize,
                              &specBufferSize,
                              &bufferSize);
        m_dspecbuf = ippsMalloc_8u (specSize);
        Ipp8u* tmp = ippsMalloc_8u (specBufferSize);
        m_dbuf     = ippsMalloc_8u (bufferSize);
        m_dpacked  = ippsMalloc_64f (m_size + 2);
        m_dspare   = ippsMalloc_64f (m_size / 2 + 1);
        ippsFFTInit_R_64f (&m_dspec,
                           m_order,
                           IPP_FFT_NODIV_BY_ANY,
                           ippAlgHintFast,
                           m_dspecbuf,
                           tmp);
        ippsFree (tmp);
#else
        int specSize, specBufferSize, bufferSize;
        ippsFFTGetSize_R_64f (m_order,
                              IPP_FFT_NODIV_BY_ANY,
                              ippAlgHintFast,
                              &specSize,
                              &specBufferSize,
                              &bufferSize);
        m_dbuf = ippsMalloc_8u (bufferSize);
        m_dpacked = ippsMalloc_64f (m_size + 2);
        m_dspare = ippsMalloc_64f (m_size / 2 + 1);
        ippsFFTInitAlloc_R_64f (
            &m_dspec, m_order, IPP_FFT_NODIV_BY_ANY, ippAlgHintFast);
#endif
    }


    bool isFloatInitialized() override { return m_fspec != nullptr; }

    bool isDoubleInitialized() override { return m_dspec != nullptr; }


    void forward (const double* BV_R_ realIn,
                  double* BV_R_       realOut,
                  double* BV_R_       imagOut) override
    {
        if (m_dspec == nullptr) initDouble();
        ippsFFTFwd_RToCCS_64f (realIn, m_dpacked, m_dspec, m_dbuf);
        unpackDouble (realOut, imagOut);
    }

    void forward (const float* BV_R_ realIn,
                  float* BV_R_       realOut,
                  float* BV_R_       imagOut) override
    {
        if (m_fspec == nullptr) initFloat();
        ippsFFTFwd_RToCCS_32f (realIn, m_fpacked, m_fspec, m_fbuf);
        unpackFloat (realOut, imagOut);
    }

    void forwardInterleaved (const double* BV_R_ realIn,
                             double* BV_R_       complexOut) override
    {
        if (m_dspec == nullptr) initDouble();
        ippsFFTFwd_RToCCS_64f (realIn, complexOut, m_dspec, m_dbuf);
    }

    void forwardInterleaved (const float* BV_R_ realIn,
                             float* BV_R_       complexOut) override
    {
        if (m_fspec == nullptr) initFloat();
        ippsFFTFwd_RToCCS_32f (realIn, complexOut, m_fspec, m_fbuf);
    }

    void forwardPolar (const double* BV_R_ realIn,
                       double* BV_R_       magOut,
                       double* BV_R_       phaseOut) override
    {
        if (m_dspec == nullptr) initDouble();
        ippsFFTFwd_RToCCS_64f (realIn, m_dpacked, m_dspec, m_dbuf);
        unpackDouble (m_dpacked, m_dspare);
        ippsCartToPolar_64f (m_dpacked, m_dspare, magOut, phaseOut, m_size / 2 + 1);
    }

    void forwardPolar (const float* BV_R_ realIn,
                       float* BV_R_       magOut,
                       float* BV_R_       phaseOut) override
    {
        if (m_fspec == nullptr) initFloat();
        ippsFFTFwd_RToCCS_32f (realIn, m_fpacked, m_fspec, m_fbuf);
        unpackFloat (m_fpacked, m_fspare);
        ippsCartToPolar_32f (m_fpacked, m_fspare, magOut, phaseOut, m_size / 2 + 1);
    }

    void forwardMagnitude (const double* BV_R_ realIn, double* BV_R_ magOut) override
    {
        if (m_dspec == nullptr) initDouble();
        ippsFFTFwd_RToCCS_64f (realIn, m_dpacked, m_dspec, m_dbuf);
        unpackDouble (m_dpacked, m_dspare);
        ippsMagnitude_64f (m_dpacked, m_dspare, magOut, m_size / 2 + 1);
    }

    void forwardMagnitude (const float* BV_R_ realIn, float* BV_R_ magOut) override
    {
        if (m_fspec == nullptr) initFloat();
        ippsFFTFwd_RToCCS_32f (realIn, m_fpacked, m_fspec, m_fbuf);
        unpackFloat (m_fpacked, m_fspare);
        ippsMagnitude_32f (m_fpacked, m_fspare, magOut, m_size / 2 + 1);
    }

    /*
 */

    void inverse (const double* BV_R_ realIn,
                  const double* BV_R_ imagIn,
                  double* BV_R_       realOut) override
    {
        if (m_dspec == nullptr) initDouble();
        packDouble (realIn, imagIn);
        ippsFFTInv_CCSToR_64f (m_dpacked, realOut, m_dspec, m_dbuf);
    }

    void inverse (const float* BV_R_ realIn,
                  const float* BV_R_ imagIn,
                  float* BV_R_       realOut) override
    {
        if (m_fspec == nullptr) initFloat();
        packFloat (realIn, imagIn);
        ippsFFTInv_CCSToR_32f (m_fpacked, realOut, m_fspec, m_fbuf);
    }

    void inverseInterleaved (const double* BV_R_ complexIn,
                             double* BV_R_       realOut) override
    {
        if (m_dspec == nullptr) initDouble();
        ippsFFTInv_CCSToR_64f (complexIn, realOut, m_dspec, m_dbuf);
    }

    void inverseInterleaved (const float* BV_R_ complexIn,
                             float* BV_R_       realOut) override
    {
        if (m_fspec == nullptr) initFloat();
        ippsFFTInv_CCSToR_32f (complexIn, realOut, m_fspec, m_fbuf);
    }

    void inversePolar (const double* BV_R_ magIn,
                       const double* BV_R_ phaseIn,
                       double* BV_R_       realOut) override
    {
        if (m_dspec == nullptr) initDouble();
        ippsPolarToCart_64f (magIn, phaseIn, realOut, m_dspare, m_size / 2 + 1);
        packDouble (realOut, m_dspare);  // to m_dpacked
        ippsFFTInv_CCSToR_64f (m_dpacked, realOut, m_dspec, m_dbuf);
    }

    void inversePolar (const float* BV_R_ magIn,
                       const float* BV_R_ phaseIn,
                       float* BV_R_       realOut) override
    {
        if (m_fspec == nullptr) initFloat();
        ippsPolarToCart_32f (magIn, phaseIn, realOut, m_fspare, m_size / 2 + 1);
        packFloat (realOut, m_fspare);  // to m_fpacked
        ippsFFTInv_CCSToR_32f (m_fpacked, realOut, m_fspec, m_fbuf);
    }

    void inverseCepstral (const double* BV_R_ magIn, double* BV_R_ cepOut) override
    {
        if (m_dspec == nullptr) initDouble();
        const int hs1 = m_size / 2 + 1;
        ippsCopy_64f (magIn, m_dspare, hs1);
        ippsAddC_64f_I (0.000001, m_dspare, hs1);
        ippsLn_64f_I (m_dspare, hs1);
        packDouble (m_dspare, 0);
        ippsFFTInv_CCSToR_64f (m_dpacked, cepOut, m_dspec, m_dbuf);
    }

    void inverseCepstral (const float* BV_R_ magIn, float* BV_R_ cepOut) override
    {
        if (m_fspec == nullptr) initFloat();
        const int hs1 = m_size / 2 + 1;
        ippsCopy_32f (magIn, m_fspare, hs1);
        ippsAddC_32f_I (0.000001f, m_fspare, hs1);
        ippsLn_32f_I (m_fspare, hs1);
        packFloat (m_fspare, 0);
        ippsFFTInv_CCSToR_32f (m_fpacked, cepOut, m_fspec, m_fbuf);
    }


private:
    BV_FORCE_INLINE void packFloat (const float* BV_R_ re, const float* BV_R_ im)
    {
        int       index = 0;
        const int hs    = m_size / 2;
        for (int i = 0; i <= hs; ++i)
        {
            m_fpacked[index++] = re[i];
            index++;
        }
        index = 0;
        if (im != nullptr)
        {
            for (int i = 0; i <= hs; ++i)
            {
                index++;
                m_fpacked[index++] = im[i];
            }
        }
        else
        {
            for (int i = 0; i <= hs; ++i)
            {
                index++;
                m_fpacked[index++] = 0.f;
            }
        }
    }

    BV_FORCE_INLINE void packDouble (const double* BV_R_ re, const double* BV_R_ im)
    {
        int       index = 0;
        const int hs    = m_size / 2;
        for (int i = 0; i <= hs; ++i)
        {
            m_dpacked[index++] = re[i];
            index++;
        }
        index = 0;
        if (im != nullptr)
        {
            for (int i = 0; i <= hs; ++i)
            {
                index++;
                m_dpacked[index++] = im[i];
            }
        }
        else
        {
            for (int i = 0; i <= hs; ++i)
            {
                index++;
                m_dpacked[index++] = 0.0;
            }
        }
    }

    BV_FORCE_INLINE void
        unpackFloat (float* re, float* BV_R_ im)  // re may be equal to m_fpacked
    {
        int       index = 0;
        const int hs    = m_size / 2;
        if (im != nullptr)
        {
            for (int i = 0; i <= hs; ++i)
            {
                index++;
                im[i] = m_fpacked[index++];
            }
        }
        index = 0;
        for (int i = 0; i <= hs; ++i)
        {
            re[i] = m_fpacked[index++];
            index++;
        }
    }

    BV_FORCE_INLINE void
        unpackDouble (double* re, double* BV_R_ im)  // re may be equal to m_dpacked
    {
        int       index = 0;
        const int hs    = m_size / 2;

        if (im != nullptr)
        {
            for (int i = 0; i <= hs; ++i)
            {
                index++;
                im[i] = m_dpacked[index++];
            }
        }
        index = 0;
        for (int i = 0; i <= hs; ++i)
        {
            re[i] = m_dpacked[index++];
            index++;
        }
    }

    /*
 */

    const int          m_size;
    int                m_order;
    IppsFFTSpec_R_32f* m_fspec;
    IppsFFTSpec_R_64f* m_dspec;
    Ipp8u*             m_fspecbuf;
    Ipp8u*             m_dspecbuf;
    Ipp8u*             m_fbuf;
    Ipp8u*             m_dbuf;
    float*             m_fpacked;
    float*             m_fspare;
    double*            m_dpacked;
    double*            m_dspare;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FFT)
};


}  // namespace bav::dsp
