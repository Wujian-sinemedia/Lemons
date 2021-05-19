/*
    FFT class using KissFFT.
*/


#include "kiss_fftr.h"


namespace bav::dsp
{
/* KissFFT only supports multiple-of-two FFT sizes */

class FFT : public FFTinterface
{
public:
    FFT (int size)
        : m_size (size)
        , m_fplanf (0)
        , m_fplani (0)
    {
        jassert (size % 2 == 0); // KissFFT only supports multiple-of-two FFT sizes
        m_fbuf    = new kiss_fft_scalar[m_size + 2];
        m_fpacked = new kiss_fft_cpx[m_size + 2];
        m_fplanf  = kiss_fftr_alloc (m_size, 0, NULL, NULL);
        m_fplani  = kiss_fftr_alloc (m_size, 1, NULL, NULL);
    }

    ~FFT() override
    {
        kiss_fftr_free (m_fplanf);
        kiss_fftr_free (m_fplani);

        delete[] m_fbuf;
        delete[] m_fpacked;
    }

    int getSize() const override { return m_size; }

    void initFloat() override { }
    void initDouble() override { }

    bool isFloatInitialized() override { return true; }

    bool isDoubleInitialized() override { return true; }


    void forward (const double* BV_R_ realIn,
                  double* BV_R_       realOut,
                  double* BV_R_       imagOut) override
    {
        vecops::convert (m_fbuf, realIn, m_size);
        kiss_fftr (m_fplanf, m_fbuf, m_fpacked);
        unpackDouble (realOut, imagOut);
    }

    void forward (const float* BV_R_ realIn,
                  float* BV_R_       realOut,
                  float* BV_R_       imagOut) override
    {
        kiss_fftr (m_fplanf, realIn, m_fpacked);
        unpackFloat (realOut, imagOut);
    }

    void forwardInterleaved (const double* BV_R_ realIn,
                             double* BV_R_       complexOut) override
    {
        vecops::convert (m_fbuf, realIn, m_size);
        kiss_fftr (m_fplanf, m_fbuf, m_fpacked);
        vecops::convert (complexOut, m_fpacked, m_size + 2);
    }

    void forwardInterleaved (const float* BV_R_ realIn,
                             float* BV_R_       complexOut) override
    {
        kiss_fftr (m_fplanf, realIn, (kiss_fft_cpx*) complexOut);
    }

    void forwardInterleaved (const float* BQ_R__ realIn,
                             float* BQ_R__       complexOut) override
    {
        kiss_fftr (m_fplanf, realIn, (kiss_fft_cpx*) complexOut);
    }

    void forwardPolar (const double* BV_R_ realIn,
                       double* BV_R_       magOut,
                       double* BV_R_       phaseOut) override
    {
        vecops::convert (m_fbuf, realIn, m_size);
        kiss_fftr (m_fplanf, m_fbuf, m_fpacked);
        vecops::cartesian_interleaved_to_polar (
            magOut, phaseOut, m_fpacked, m_size / 2 + 1);
    }

    void forwardPolar (const float* BV_R_ realIn,
                       float* BV_R_       magOut,
                       float* BV_R_       phaseOut) override
    {
        kiss_fftr (m_fplanf, realIn, m_fpacked);
        vecops::cartesian_interleaved_to_polar (
            magOut, phaseOut, m_fpacked, m_size / 2 + 1);
    }

    void forwardMagnitude (const double* BV_R_ realIn, double* BV_R_ magOut) override
    {
        vecops::convert (m_fbuf, realIn, m_size);
        kiss_fftr (m_fplanf, m_fbuf, m_fpacked);
        vecops::cartesian_interleaved_to_magnitudes (
            magOut, (float*) m_fpacked, m_size / 2 + 1);
    }

    void forwardMagnitude (const float* BV_R_ realIn, float* BV_R_ magOut) override
    {
        kiss_fftr (m_fplanf, realIn, m_fpacked);
        vecops::cartesian_interleaved_to_magnitudes (
            magOut, (float*) m_fpacked, m_size / 2 + 1);
    }

    /*
        */

    void inverse (const double* BV_R_ realIn,
                  const double* BV_R_ imagIn,
                  double* BV_R_       realOut)
    {
        packDouble (realIn, imagIn);
        kiss_fftri (m_fplani, m_fpacked, m_fbuf);
        vecops::convert (realOut, m_fbuf, m_size);
    }

    void inverse (const float* BV_R_ realIn,
                  const float* BV_R_ imagIn,
                  float* BV_R_       realOut) override
    {
        packFloat (realIn, imagIn);
        kiss_fftri (m_fplani, m_fpacked, realOut);
    }

    void inverseInterleaved (const double* BV_R_ complexIn,
                             double* BV_R_       realOut) override
    {
        vecops::convert (m_fpacked, complexIn, m_size + 2);
        kiss_fftri (m_fplani, m_fpacked, m_fbuf);
        vecops::convert (realOut, m_fbuf, m_size);
    }

    void inverseInterleaved (const float* BV_R_ complexIn,
                             float* BV_R_       realOut) override
    {
        vecops::copy (m_fpacked, complexIn, m_size + 2);
        kiss_fftri (m_fplani, m_fpacked, realOut);
    }

    void inversePolar (const double* BV_R_ magIn,
                       const double* BV_R_ phaseIn,
                       double* BV_R_       realOut) override
    {
        vecops::polar_to_cartesian_interleaved (
            m_fpacked, magIn, phaseIn, m_size / 2 + 1);
        kiss_fftri (m_fplani, m_fpacked, m_fbuf);
        vecops::convert (realOut, m_fbuf, m_size);
    }

    void inversePolar (const float* BV_R_ magIn,
                       const float* BV_R_ phaseIn,
                       float* BV_R_       realOut) override
    {
        vecops::polar_to_cartesian_interleaved (
            m_fpacked, magIn, phaseIn, m_size / 2 + 1);
        kiss_fftri (m_fplani, m_fpacked, realOut);
    }

    void inverseCepstral (const double* BV_R_ magIn, double* BV_R_ cepOut) override
    {
        const int hs = m_size / 2;
        for (int i = 0; i <= hs; ++i)
        {
            m_fpacked[i].r = float (log (magIn[i] + 0.000001));
            m_fpacked[i].i = 0.0f;
        }
        kiss_fftri (m_fplani, m_fpacked, m_fbuf);
        vecops::convert (cepOut, m_fbuf, m_size);
    }

    void inverseCepstral (const float* BV_R_ magIn, float* BV_R_ cepOut) override
    {
        const int hs = m_size / 2;
        for (int i = 0; i <= hs; ++i)
        {
            m_fpacked[i].r = logf (magIn[i] + 0.000001f);
            m_fpacked[i].i = 0.0f;
        }
        kiss_fftri (m_fplani, m_fpacked, cepOut);
    }

    /*
        */

private:
    BV_FORCE_INLINE void packFloat (const float* BV_R_ re, const float* BV_R_ im)
    {
        const int hs = m_size / 2;
        for (int i = 0; i <= hs; ++i)
        {
            m_fpacked[i].r = re[i];
        }
        if (im != nullptr)
        {
            for (int i = 0; i <= hs; ++i)
            {
                m_fpacked[i].i = im[i];
            }
        }
        else
        {
            for (int i = 0; i <= hs; ++i)
            {
                m_fpacked[i].i = 0.f;
            }
        }
    }

    BV_FORCE_INLINE void unpackFloat (float* BV_R_ re, float* BV_R_ im)
    {
        const int hs = m_size / 2;
        for (int i = 0; i <= hs; ++i)
        {
            re[i] = m_fpacked[i].r;
        }
        if (im != nullptr)
        {
            for (int i = 0; i <= hs; ++i)
            {
                im[i] = m_fpacked[i].i;
            }
        }
    }

    BV_FORCE_INLINE void packDouble (const double* BV_R_ re, const double* BV_R_ im)
    {
        const int hs = m_size / 2;
        for (int i = 0; i <= hs; ++i)
        {
            m_fpacked[i].r = float (re[i]);
        }
        if (im != nullptr)
        {
            for (int i = 0; i <= hs; ++i)
            {
                m_fpacked[i].i = float (im[i]);
            }
        }
        else
        {
            for (int i = 0; i <= hs; ++i)
            {
                m_fpacked[i].i = 0.f;
            }
        }
    }

    BV_FORCE_INLINE void unpackDouble (double* BQ_R__ re, double* BQ_R__ im)
    {
        const int hs = m_size / 2;
        for (int i = 0; i <= hs; ++i)
        {
            re[i] = double (m_fpacked[i].r);
        }
        if (im != nullptr)
        {
            for (int i = 0; i <= hs; ++i)
            {
                im[i] = double (m_fpacked[i].i);
            }
        }
    }

    const int        m_size;
    kiss_fftr_cfg    m_fplanf;
    kiss_fftr_cfg    m_fplani;
    kiss_fft_scalar* m_fbuf;
    kiss_fft_cpx*    m_fpacked;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FFT)
};


} // namespace bav::dsp
