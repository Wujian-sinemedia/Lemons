/*
 FFT class using FFTW.
*/


/*
 Define FFTW_DOUBLE_ONLY to make all uses of FFTW functions be
 double-precision (so "float" FFTs are calculated by casting to
 doubles and using the double-precision FFTW function).
 Define FFTW_SINGLE_ONLY to make all uses of FFTW functions be
 single-precision (so "double" FFTs are calculated by casting to
 floats and using the single-precision FFTW function).
 Neither of these flags is desirable for either performance or
 precision. The main reason to define either flag is to avoid linking
 against both fftw3 and fftw3f libraries.
*/

/*
 Because FFTW builds in double-precision by default, our default
 behavior here is to assume that our available version of FFTW is
 double-only, unless we have been explicitly told otherwise.
*/

#ifndef FFTW_SINGLE_ONLY
  #ifndef FFTW_DOUBLE_ONLY
    #define FFTW_DOUBLE_ONLY 1
  #endif
#endif


/* Can't meaningfully define both... */
#if defined(FFTW_DOUBLE_ONLY) && defined(FFTW_SINGLE_ONLY)
  #error Can only define one of FFTW_DOUBLE_ONLY or FFTW_SINGLE_ONLY
#endif

#ifdef FFTW_DOUBLE_ONLY
  #define fft_float_type double
  #define fftwf_complex fftw_complex
  #define fftwf_plan fftw_plan
  #define fftwf_plan_dft_r2c_1d fftw_plan_dft_r2c_1d
  #define fftwf_plan_dft_c2r_1d fftw_plan_dft_c2r_1d
  #define fftwf_destroy_plan fftw_destroy_plan
  #define fftwf_malloc fftw_malloc
  #define fftwf_free fftw_free
  #define fftwf_execute fftw_execute
  #define atan2f atan2
  #define sqrtf sqrt
  #define cosf cos
  #define sinf sin
#else
  #define fft_float_type float
#endif /* FFTW_DOUBLE_ONLY */

#ifdef FFTW_SINGLE_ONLY
  #define fft_double_type float
  #define fftw_complex fftwf_complex
  #define fftw_plan fftwf_plan
  #define fftw_plan_dft_r2c_1d fftwf_plan_dft_r2c_1d
  #define fftw_plan_dft_c2r_1d fftwf_plan_dft_c2r_1d
  #define fftw_destroy_plan fftwf_destroy_plan
  #define fftw_malloc fftwf_malloc
  #define fftw_free fftwf_free
  #define fftw_execute fftwf_execute
  #define atan2 atan2f
  #define sqrt sqrtf
  #define cos cosf
  #define sin sinf
#else
  #define fft_double_type double
#endif /* FFTW_SINGLE_ONLY */


namespace bav::dsp
{
    

/* FFTW supports any FFT size */
    
class FFT : public FFTinterface
{
public:
    FFT(int size) :
        m_fplanf(0), m_dplanf(0), m_size(size)
    {
    }
    
    ~FFT() override
    {
        if (m_fplanf)
        {
            fftwf_destroy_plan (m_fplanf);
            fftwf_destroy_plan (m_fplani);
            fftwf_free (m_fbuf);
            fftwf_free (m_fpacked);
        }
        if (m_dplanf)
        {
            fftw_destroy_plan (m_dplanf);
            fftw_destroy_plan (m_dplani);
            fftw_free (m_dbuf);
            fftw_free (m_dpacked);
        }
        if (m_extantf <= 0 && m_extantd <= 0) {
#ifndef FFTW_DOUBLE_ONLY
            fftwf_cleanup();
#endif
#ifndef FFTW_SINGLE_ONLY
            fftw_cleanup();
#endif
        }
    }
    
    int getSize() const override
    {
        return m_size;
    }
    
    bool isFloatInitialized() override
    {
        return m_fplanf != nullptr;
    }
    
    bool isDoubleInitialized() override
    {
        return m_dplanf != nullptr;
    }
    
    void initFloat() override
    {
        if (m_fplanf != nullptr) return;
        
        m_fbuf = (fft_float_type *) fftw_malloc (m_size * sizeof(fft_float_type));
        m_fpacked = (fftwf_complex *) fftw_malloc ((m_size/2 + 1) * sizeof(fftwf_complex));

        m_fplanf = fftwf_plan_dft_r2c_1d (m_size, m_fbuf, m_fpacked, FFTW_ESTIMATE);
        m_fplani = fftwf_plan_dft_c2r_1d (m_size, m_fpacked, m_fbuf, FFTW_ESTIMATE);
    }
    
    void initDouble() override
    {
        if (m_dplanf != nullptr) return;
        
        m_dbuf = (fft_double_type *) fftw_malloc (m_size * sizeof(fft_double_type));
        m_dpacked = (fftw_complex *) fftw_malloc ((m_size/2 + 1) * sizeof(fftw_complex));

        m_dplanf = fftw_plan_dft_r2c_1d (m_size, m_dbuf, m_dpacked, FFTW_ESTIMATE);
        m_dplani = fftw_plan_dft_c2r_1d (m_size, m_dpacked, m_dbuf, FFTW_ESTIMATE);
    }
    
    
    void forward (const double* BV_R_ realIn, double* BV_R_ realOut, double* BV_R_ imagOut) override
    {
        if (m_dplanf == nullptr) initDouble();
        
        const int sz = m_size;
        fft_double_type* const BV_R_ dbuf = m_dbuf;
#ifndef FFTW_SINGLE_ONLY
        if (realIn != dbuf)
#endif
            for (int i = 0; i < sz; ++i) {
                dbuf[i] = realIn[i];
            }
        fftw_execute (m_dplanf);
        unpackDouble (realOut, imagOut);
    }
    
    void forward (const float* BV_R_ realIn, float* BV_R_ realOut, float* BV_R_ imagOut) override
    {
        if (m_fplanf == nullptr) initFloat();
        
        fft_float_type* const BV_R_ fbuf = m_fbuf;
        const int sz = m_size;
#ifndef FFTW_DOUBLE_ONLY
        if (realIn != fbuf)
#endif
            for (int i = 0; i < sz; ++i) {
                fbuf[i] = realIn[i];
            }
        fftwf_execute (m_fplanf);
        unpackFloat (realOut, imagOut);
    }
    
    void forwardInterleaved (const double* BV_R_ realIn, double* BV_R_ complexOut) override
    {
        if (m_dplanf == nullptr) initDouble();
        
        const int sz = m_size;
        fft_double_type* const BV_R_ dbuf = m_dbuf;
#ifndef FFTW_SINGLE_ONLY
        if (realIn != dbuf)
#endif
            for (int i = 0; i < sz; ++i) {
                dbuf[i] = realIn[i];
            }
        fftw_execute (m_dplanf);
        
        vecops::convert (complexOut, (const fft_double_type *)m_dpacked, sz + 2);
    }
    
    void forwardInterleaved (const float* BV_R_ realIn, float* BV_R_ complexOut) override
    {
        if (m_fplanf == nullptr) initFloat();
        
        fft_float_type* const BV_R_ fbuf = m_fbuf;
        const int sz = m_size;
#ifndef FFTW_DOUBLE_ONLY
        if (realIn != fbuf)
#endif
            for (int i = 0; i < sz; ++i) {
                fbuf[i] = realIn[i];
            }
        fftwf_execute (m_fplanf);
        
        vecops::convert (complexOut, (const fft_float_type *)m_fpacked, sz + 2);
    }
    
    void forwardPolar (const double* BV_R_ realIn, double* BV_R_ magOut, double* BV_R_ phaseOut) override
    {
        if (m_dplanf == nullptr) initDouble();
        
        fft_double_type* const BV_R_ dbuf = m_dbuf;
        const int sz = m_size;
#ifndef FFTW_SINGLE_ONLY
        if (realIn != dbuf)
#endif
            for (int i = 0; i < sz; ++i) {
                dbuf[i] = realIn[i];
            }
        fftw_execute (m_dplanf);
        vecops::cartesian_interleaved_to_polar (magOut, phaseOut, (const fft_double_type *)m_dpacked, m_size/2+1);
    }
    
    void forwardPolar (const float* BV_R_ realIn, float* BV_R_ magOut, float* BV_R_ phaseOut) override
    {
        if (m_fplanf == nullptr) initFloat();
        
        fft_float_type* const BV_R_ fbuf = m_fbuf;
        const int sz = m_size;
#ifndef FFTW_DOUBLE_ONLY
        if (realIn != fbuf)
#endif
            for (int i = 0; i < sz; ++i) {
                fbuf[i] = realIn[i];
            }
        fftwf_execute (m_fplanf);
        vecops::cartesian_interleaved_to_polar (magOut, phaseOut, (const fft_float_type *)m_fpacked, m_size/2+1);
    }
    
    void forwardMagnitude (const double* BV_R_ realIn, double* BV_R_ magOut) override
    {
        if (m_dplanf == nullptr) initDouble();
        
        fft_double_type* const BV_R_ dbuf = m_dbuf;
        const int sz = m_size;
#ifndef FFTW_SINGLE_ONLY
        if (realIn != m_dbuf)
#endif
            for (int i = 0; i < sz; ++i) {
                dbuf[i] = realIn[i];
            }
        fftw_execute (m_dplanf);
        vecops::cartesian_interleaved_to_magnitudes (magOut, (const fft_double_type *)m_dpacked, m_size/2+1);
    }
    
    void forwardMagnitude (const float* BV_R_ realIn, float* BV_R_ magOut) override
    {
        if (m_fplanf == nullptr) initFloat();
        
        fft_float_type* const BV_R_ fbuf = m_fbuf;
        const int sz = m_size;
#ifndef FFTW_DOUBLE_ONLY
        if (realIn != fbuf)
#endif
            for (int i = 0; i < sz; ++i) {
                fbuf[i] = realIn[i];
            }
        fftwf_execute (m_fplanf);
        vecops::cartesian_interleaved_to_magnitudes (magOut, (const fft_float_type *)m_fpacked, m_size/2+1);
    }
    
    /*
    */
    
    void inverse (const double* BV_R_ realIn, const double* BV_R_ imagIn, double* BV_R_ realOut) override
    {
        if (m_dplanf == nullptr) initDouble();
        
        packDouble (realIn, imagIn);
        fftw_execute (m_dplani);
        const int sz = m_size;
        fft_double_type* const BV_R_ dbuf = m_dbuf;
#ifndef FFTW_SINGLE_ONLY
        if (realOut != dbuf)
#endif
            for (int i = 0; i < sz; ++i) {
                realOut[i] = dbuf[i];
            }
    }
    
    void inverse (const float* BV_R_ realIn, const float* BV_R_ imagIn, float* BV_R_ realOut) override
    {
        if (m_fplanf == nullptr) initFloat();
        
        packFloat (realIn, imagIn);
        fftwf_execute (m_fplani);
        const int sz = m_size;
        fft_float_type* const BQ_R__ fbuf = m_fbuf;
#ifndef FFTW_DOUBLE_ONLY
        if (realOut != fbuf)
#endif
            for (int i = 0; i < sz; ++i) {
                realOut[i] = fbuf[i];
            }
    }
    
    void inverseInterleaved (const double* BV_R_ complexIn, double* BV_R_ realOut) override
    {
        if (m_dplanf == nullptr) initDouble();
        
        vecops::convert ((fft_double_type *)m_dpacked, complexIn, m_size + 2);
        
        fftw_execute (m_dplani);
        const int sz = m_size;
        fft_double_type* const BV_R_ dbuf = m_dbuf;
#ifndef FFTW_SINGLE_ONLY
        if (realOut != dbuf)
#endif
            for (int i = 0; i < sz; ++i) {
                realOut[i] = dbuf[i];
            }
    }
    
    void inverseInterleaved (const float* BV_R_ complexIn, float* BV_R_ realOut) override
    {
        if (m_fplanf == nullptr) initFloat();
        
        vecops::convert ((fft_float_type *)m_fpacked, complexIn, m_size + 2);
        
        fftwf_execute (m_fplani);
        const int sz = m_size;
        fft_float_type* const BV_R_ fbuf = m_fbuf;
#ifndef FFTW_DOUBLE_ONLY
        if (realOut != fbuf)
#endif
            for (int i = 0; i < sz; ++i) {
                realOut[i] = fbuf[i];
            }
    }
    
    void inversePolar (const double* BV_R_ magIn, const double* BV_R_ phaseIn, double* BV_R_ realOut) override
    {
        if (m_dplanf == nullptr) initDouble();
        
        vecops::polar_to_cartesian_interleaved ((fft_double_type *)m_dpacked, magIn, phaseIn, m_size/2+1);
        fftw_execute (m_dplani);
        const int sz = m_size;
        fft_double_type* const BV_R_ dbuf = m_dbuf;
#ifndef FFTW_SINGLE_ONLY
        if (realOut != dbuf)
#endif
            for (int i = 0; i < sz; ++i) {
                realOut[i] = dbuf[i];
            }
    }
    
    void inversePolar (const float* BV_R_ magIn, const float* BV_R_ phaseIn, float* BV_R_ realOut) override
    {
        if (m_fplanf == nullptr) initFloat();
        
        vecops::polar_to_cartesian_interleaved ((fft_float_type *)m_fpacked, magIn, phaseIn, m_size/2+1);
        fftwf_execute (m_fplani);
        const int sz = m_size;
        fft_float_type* const BV_R_ fbuf = m_fbuf;
#ifndef FFTW_DOUBLE_ONLY
        if (realOut != fbuf)
#endif
            for (int i = 0; i < sz; ++i) {
                realOut[i] = fbuf[i];
            }
    }
    
    void inverseCepstral (const double* BV_R_ magIn, double* BV_R_ cepOut) override
    {
        if (m_dplanf == nullptr) initDouble();
        
        fft_double_type* const BV_R_ dbuf = m_dbuf;
        fftw_complex* const BV_R_ dpacked = m_dpacked;
        const int hs = m_size/2;
        for (int i = 0; i <= hs; ++i) {
            dpacked[i][0] = log(magIn[i] + 0.000001);
        }
        for (int i = 0; i <= hs; ++i) {
            dpacked[i][1] = 0.0;
        }
        fftw_execute (m_dplani);
        const int sz = m_size;
#ifndef FFTW_SINGLE_ONLY
        if (cepOut != dbuf)
#endif
            for (int i = 0; i < sz; ++i) {
                cepOut[i] = dbuf[i];
            }
    }
    
    void inverseCepstral (const float* BV_R_ magIn, float* BV_R_ cepOut) override
    {
        if (m_fplanf == nullptr) initFloat();
        
        const int hs = m_size/2;
        fftwf_complex* const BV_R_ fpacked = m_fpacked;
        for (int i = 0; i <= hs; ++i) {
            fpacked[i][0] = logf(magIn[i] + 0.000001f);
        }
        for (int i = 0; i <= hs; ++i) {
            fpacked[i][1] = 0.f;
        }
        fftwf_execute (m_fplani);
        const int sz = m_size;
        fft_float_type* const BV_R_ fbuf = m_fbuf;
#ifndef FFTW_DOUBLE_ONLY
        if (cepOut != fbuf)
#endif
            for (int i = 0; i < sz; ++i) {
                cepOut[i] = fbuf[i];
            }
    }
    
    
private:
    void packFloat (const float* BV_R_ re, const float* BV_R_ im)
    {
        const int hs = m_size/2;
        fftwf_complex* const BV_R_ fpacked = m_fpacked;
        for (int i = 0; i <= hs; ++i) {
            fpacked[i][0] = re[i];
        }
        if (im != nullptr) {
            for (int i = 0; i <= hs; ++i) {
                fpacked[i][1] = im[i];
            }
        } else {
            for (int i = 0; i <= hs; ++i) {
                fpacked[i][1] = 0.f;
            }
        }
    }
    
    void packDouble (const double* BV_R_ re, const double* BV_R_ im)
    {
        const int hs = m_size/2;
        fftw_complex* const BV_R_ dpacked = m_dpacked;
        for (int i = 0; i <= hs; ++i) {
            dpacked[i][0] = re[i];
        }
        if (im != nullptr) {
            for (int i = 0; i <= hs; ++i) {
                dpacked[i][1] = im[i];
            }
        } else {
            for (int i = 0; i <= hs; ++i) {
                dpacked[i][1] = 0.0;
            }
        }
    }
    
    void unpackFloat (float* BV_R_ re, float* BV_R_ im)
    {
        const int hs = m_size/2;
        for (int i = 0; i <= hs; ++i) {
            re[i] = m_fpacked[i][0];
        }
        if (im != nullptr) {
            for (int i = 0; i <= hs; ++i) {
                im[i] = m_fpacked[i][1];
            }
        }
    }
    
    void unpackDouble (double* BV_R_ re, double* BV_R_ im)
    {
        const int hs = m_size/2;
        for (int i = 0; i <= hs; ++i) {
            re[i] = m_dpacked[i][0];
        }
        if (im != nullptr) {
            for (int i = 0; i <= hs; ++i) {
                im[i] = m_dpacked[i][1];
            }
        }
    }
    
    /*
    */
    
    fftwf_plan m_fplanf;
    fftwf_plan m_fplani;
#ifdef FFTW_DOUBLE_ONLY
    double* m_fbuf;
#else
    float* m_fbuf;
#endif
    fftwf_complex *m_fpacked;
    fftw_plan m_dplanf;
    fftw_plan m_dplani;
#ifdef FFTW_SINGLE_ONLY
    float* m_dbuf;
#else
    double* m_dbuf;
#endif
    fftw_complex *m_dpacked;
    const int m_size;
    static int m_extantf;
    static int m_extantd;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FFT)
};

#undef fft_float_type
#undef fft_double_type

#ifdef FFTW_DOUBLE_ONLY
  #undef fftwf_complex
  #undef fftwf_plan
  #undef fftwf_plan_dft_r2c_1d
  #undef fftwf_plan_dft_c2r_1d
  #undef fftwf_destroy_plan
  #undef fftwf_malloc
  #undef fftwf_free
  #undef fftwf_execute
  #undef atan2f
  #undef sqrtf
  #undef cosf
  #undef sinf
#endif /* FFTW_DOUBLE_ONLY */

#ifdef FFTW_SINGLE_ONLY
  #undef fftw_complex
  #undef fftw_plan
  #undef fftw_plan_dft_r2c_1d
  #undef fftw_plan_dft_c2r_1d
  #undef fftw_destroy_plan
  #undef fftw_malloc
  #undef fftw_free
  #undef fftw_execute
  #undef atan2
  #undef sqrt
  #undef cos
  #undef sin
#endif /* FFTW_SINGLE_ONLY */


}  // namespace
