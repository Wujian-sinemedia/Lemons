
/*
    FFT class using Ne10.
*/

namespace bav::dsp
{

    
    class FFT :     public FFTinterface
    {
    public:
        FFT(int size): m_size(size)
        {
            
        }
        
        ~FFT() override
        {
            if (config != nullptr)
                ne10_fft_destroy_r2c_float32 (config);
            
            free (conversionBuf);
            free (workingBuf);
        }
        
        int getSize() const override { return 0; }
        
        void initFloat() override
        {
            ne10_init();
            
            if (config != nullptr)
                config = ne10_fft_alloc_r2c_float32 (m_size);
        }
        
        void initDouble() override
        {
            ne10_init();
            
            if (config != nullptr)
                config = ne10_fft_alloc_r2c_float32 (m_size);
            
            if (conversionBuf == nullptr)
                conversionBuf = malloc(m_size * sizeof(float));
            
            if (workingBuf == nullptr)
                workingBuf = malloc(m_size * sizeof(double));
        }
        
        bool isFloatInitialized()  override { return config != nullptr; }
        bool isDoubleInitialized() override { return config != nullptr; }
        
        void forward (const double* BV_R_ realIn, double* BV_R_ realOut, double* BV_R_ imagOut) override
        {
            if (config == nullptr)
                initDouble();
            
            vecops::convert (conversionBuf, realIn, m_size);
            
            ne10_fft_r2c_1d_float32 (temp, conversionBuf, config);
            
            for (int i = 0; i <= m_size/2; ++i) {
                realOut[i] = temp[i].r;
                imagOut[i] = temp[i].i;
            }
        }
        
        void forward (const float* BV_R_ realIn, float* BV_R_ realOut, float* BV_R_ imagOut) override
        {
            if (config == nullptr)
                initFloat();
            
            ne10_fft_r2c_1d_float32 (temp, realIn, config);
            
            for (int i = 0; i <= m_size/2; ++i) {
                realOut[i] = temp[i].r;
                imagOut[i] = temp[i].i;
            }
        }
        
        void forwardInterleaved (const double* BV_R_ realIn, double* BV_R_ complexOut) override
        {
            if (config == nullptr)
                initDouble();
            
            vecops::convert (conversionBuf, realIn, m_size);
            
            ne10_fft_r2c_1d_float32 (temp, conversionBuf, config);
            
            for (int i = 0; i <= m_size; i += 2) {
                complexOut[i]   = temp[i].r;
                complexOut[i+1] = temp[i].i;
            }
        }
        
        void forwardInterleaved (const float* BV_R_ realIn, float* BV_R_ complexOut) override
        {
            if (config == nullptr)
                initFloat();
            
            ne10_fft_r2c_1d_float32 (temp, realIn, config);
            
            for (int i = 0; i <= m_size; i += 2) {
                complexOut[i]   = temp[i].r;
                complexOut[i+1] = temp[i].i;
            }
        }
        
        void forwardPolar (const double* BV_R_ realIn, double* BV_R_ magOut, double* BV_R_ phaseOut) override
        {
            if (config == nullptr)
                initDouble();
            
            vecops::convert (conversionBuf, realIn, m_size);
            
            ne10_fft_r2c_1d_float32 (temp, conversionBuf, config);
            
            for (int i = 0; i <= m_size; i += 2) {
                workingBuf[i]   = double(temp[i].r);
                workingBuf[i+1] = double(temp[i].i);
            }
            
            vecops::cartesian_interleaved_to_polar (magOut, phaseOut, workingBuf, m_size/2 + 1);
        }
        
        void forwardPolar (const float* BV_R_ realIn, float* BV_R_ magOut, float* BV_R_ phaseOut) override
        {
            if (config == nullptr)
                initFloat();
            
            ne10_fft_r2c_1d_float32 (temp, realIn, config);
            
            for (int i = 0; i <= m_size; i += 2) {
                conversionBuf[i]   = temp[i].r;
                conversionBuf[i+1] = temp[i].i;
            }
            
            vecops::cartesian_interleaved_to_polar (magOut, phaseOut, conversionBuf, m_size/2 + 1);
        }
        
        void forwardMagnitude (const double* BV_R_ realIn, double* BV_R_ magOut) override
        {
            if (config == nullptr)
                initDouble();
            
            vecops::convert (conversionBuf, realIn, m_size);
            
            ne10_fft_r2c_1d_float32 (temp, conversionBuf, config);
            
            for (int i = 0; i <= m_size; i += 2) {
                workingBuf[i]   = double(temp[i].r);
                workingBuf[i+1] = double(temp[i].i);
            }
            
            vecops::cartesian_interleaved_to_magnitudes (magOut, workingBuf, m_size/2 + 1);
        }
        
        void forwardMagnitude (const float* BV_R_ realIn, float* BV_R_ magOut) override
        {
            if (config == nullptr)
                initFloat();
            
            ne10_fft_r2c_1d_float32 (temp, realIn, config);
            
            for (int i = 0; i <= m_size; i += 2) {
                conversionBuf[i]   = temp[i].r;
                conversionBuf[i+1] = temp[i].i;
            }
            
            vecops::cartesian_interleaved_to_magnitudes (magOut, conversionBuf, m_size/2 + 1);
        }
        
        /*
        */
        
        void inverse (const double* BV_R_ realIn, const double* BV_R_ imagIn, double* BV_R_ realOut) override
        {
            if (config == nullptr)
                initDouble();
            
            for (int i = 0; i <= m_size; ++i) {
                temp[i].r = float(realIn[i]);
                temp[i].i = float(imagIn[i]);
            }
            
            ne10_fft_c2r_1d_float32 (realOut, temp, config);
        }
        
        void inverse (const float* BV_R_ realIn, const float* BV_R_ imagIn, float* BV_R_ realOut) override
        {
            if (config == nullptr)
                initFloat();
            
            for (int i = 0; i <= m_size; ++i) {
                temp[i].r = realIn[i];
                temp[i].i = imagIn[i];
            }
            
            ne10_fft_c2r_1d_float32 (realOut, temp, config);
        }
        
        void inverseInterleaved (const double* BV_R_ complexIn, double* BV_R_ realOut) override
        {
            if (config == nullptr)
                initDouble();
            
            for (int i = 0; i <= m_size; i += 2) {
                temp[i].r = float(complexIn[i]);
                temp[i].i = float(complexIn[i+1]);
            }
            
            ne10_fft_c2r_1d_float32 (conversionBuf, temp, config);
            
            vecops::convert (realOut, conversionBuf, m_size);
        }
        
        void inverseInterleaved (const float* BV_R_ complexIn, float* BV_R_ realOut) override
        {
            if (config == nullptr)
                initFloat();
            
            for (int i = 0; i <= m_size; i += 2) {
                temp[i].r = complexIn[i];
                temp[i].i = complexIn[i+1];
            }
            
            ne10_fft_c2r_1d_float32 (realOut, temp, config);
        }
        
        void inversePolar (const double* BV_R_ magIn, const double* BV_R_ phaseIn, double* BV_R_ realOut) override
        {
            if (config == nullptr)
                initDouble();
            
            vecops::polar_to_cartesian_interleaved (workingBuf, magIn, phaseIn, m_size/2 + 1);
            
            for (int i = 0; i <= m_size; i += 2) {
                temp[i].r = float(workingBuf[i]);
                temp[i].i = float(workingBuf[i+1]);
            }
            
            ne10_fft_c2r_1d_float32 (conversionBuf, temp, config);
            
            vecops::convert (realOut, conversionBuf, m_size);
        }
        
        void inversePolar (const float* BV_R_ magIn, const float* BV_R_ phaseIn, float* BV_R_ realOut) override
        {
            if (config == nullptr)
                initFloat();
            
            vecops::polar_to_cartesian_interleaved (conversionBuf, magIn, phaseIn, m_size/2 + 1);
            
            for (int i = 0; i <= m_size; i += 2) {
                temp[i].r = conversionBuf[i];
                temp[i].i = conversionBuf[i+1];
            }
            
            ne10_fft_c2r_1d_float32 (realOut, temp, config);
        }
        
        void inverseCepstral (const double* BV_R_ magIn, double* BV_R_ cepOut) override
        {
            if (config == nullptr)
                initDouble();
            
            for (int i = 0; i <= m_size/2; ++i) {
                temp[i].r = float (log(magIn[i] + 0.000001));
                temp[i].i = 0.0f
            }
            
            ne10_fft_c2r_1d_float32 (conversionBuf, temp, config);
            
            vecops::convert (cepOut, conversionBuf, m_size);
        }
        
        void inverseCepstral (const float* BV_R_ magIn, float* BV_R_ cepOut) override
        {
            if (config == nullptr)
                initFloat();
            
            for (int i = 0; i <= m_size/2; ++i) {
                temp[i].r = logf (magIn[i] + 0.000001f);
                temp[i].i = 0.0f
            }
            
            ne10_fft_c2r_1d_float32 (cepOut, temp, config);
        }
        
        /*
        */
        
    private:
        int32_t m_size;
        
        ne10_fft_r2c_cfg_float32_t* config;
        
        ne10_fft_cpx_float32_t temp[m_size/2 + 1] = {};  // A destination array for the transformed data
        
        float* conversionBuf;
        double* workingBuf;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FFT)
    };
    
    
}  // namespace
