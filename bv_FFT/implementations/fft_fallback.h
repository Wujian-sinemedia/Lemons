/*
    Fallback FFT class written in pure C++.
*/


namespace bav::dsp
{
    

class FFT : public FFTinterface
{
public:
    FFT(int size) :
    m_size(size),
    m_half(size/2),
    m_blockTableSize(16),
    m_maxTabledBlock(1 << m_blockTableSize)
    {
        const auto half = size_t(m_half);
        m_table = aligned_allocate_zero<int>(half);
        m_sincos = aligned_allocate_zero<double>(size_t(m_blockTableSize * 4));
        m_sincos_r = aligned_allocate_zero<double>(half);
        m_vr = aligned_allocate_zero<double>(half);
        m_vi = aligned_allocate_zero<double>(half);
        m_a = aligned_allocate_zero<double>(half + 1);
        m_b = aligned_allocate_zero<double>(half + 1);
        m_c = aligned_allocate_zero<double>(half + 1);
        m_d = aligned_allocate_zero<double>(half + 1);
        m_e = aligned_allocate_zero<double>(half + 1);
        m_f = aligned_allocate_zero<double>(half + 1);
        m_a_and_b[0] = m_a;
        m_a_and_b[1] = m_b;
        m_c_and_d[0] = m_c;
        m_c_and_d[1] = m_d;
        makeTables();
    }
    
    ~FFT() override
    {
        aligned_deallocate (m_table);
        aligned_deallocate (m_sincos);
        aligned_deallocate (m_sincos_r);
        aligned_deallocate (m_vr);
        aligned_deallocate (m_vi);
        aligned_deallocate (m_a);
        aligned_deallocate (m_b);
        aligned_deallocate (m_c);
        aligned_deallocate (m_d);
        aligned_deallocate (m_e);
        aligned_deallocate (m_f);
    }
    
    int getSize() const override
    {
        return m_size;
    }
    
    void initFloat() override { }
    void initDouble() override { }
    
    bool isFloatInitialized() override { return true; }
    
    bool isDoubleInitialized() override { return true; }
    
    
    void forward (const double* BV_R_ realIn, double* BV_R_ realOut, double* BV_R_ imagOut) override
    {
        transformF (realIn, realOut, imagOut);
    }
    
    void forward (const float* BV_R_ realIn, float* BV_R_ realOut, float* BV_R_ imagOut) override
    {
        transformF (realIn, m_c, m_d);
        vecops::convert (realOut, m_c, m_half + 1);
        vecops::convert (imagOut, m_d, m_half + 1);
    }
    
    void forwardInterleaved (const double* BV_R_ realIn, double* BV_R_ complexOut) override
    {
        transformF (realIn, m_c, m_d);
        vecops::interleave (complexOut, m_c_and_d, 2, m_half + 1);
    }
    
    void forwardInterleaved (const float* BV_R_ realIn, float* BV_R_ complexOut) override
    {
        transformF (realIn, m_c, m_d);
        
        for (int i = 0; i <= m_half; ++i) complexOut[i*2] = float(m_c[i]);
        for (int i = 0; i <= m_half; ++i) complexOut[i*2+1] = float(m_d[i]);
    }
    
    void forwardPolar (const double* BV_R_ realIn, double* BV_R_ magOut, double* BV_R_ phaseOut) override
    {
        transformF (realIn, m_c, m_d);
        vecops::cartesian_to_polar (magOut, phaseOut, m_c, m_d, m_half + 1);
    }
    
    void forwardPolar (const float* BV_R_ realIn, float* BV_R_ magOut, float* BV_R_ phaseOut) override
    {
        transformF (realIn, m_c, m_d);
        vecops::cartesian_to_polar (m_e, m_f, m_c, m_d, m_half + 1);
        vecops::convert (magOut, m_e, m_half + 1);
        vecops::convert (phaseOut, m_f, m_half + 1);
    }
    
    void forwardMagnitude (const double* BV_R_ realIn, double* BV_R_ magOut) override
    {
        transformF (realIn, m_c, m_d);
        vecops::cartesian_to_magnitudes (magOut, m_c, m_d, m_half + 1);
    }
    
    void forwardMagnitude (const float* BV_R_ realIn, float* BV_R_ magOut) override
    {
        transformF (realIn, m_c, m_d);
        vecops::cartesian_to_magnitudes (m_e, m_c, m_d, m_half + 1);
        vecops::convert (magOut, m_e, m_half + 1);
    }
    
    void inverse (const double* BV_R_ realIn, const double* BV_R_ imagIn, double* BV_R_ realOut) override
    {
        transformI (realIn, imagIn, realOut);
    }
    
    void inverse (const float* BV_R_ realIn, const float* BV_R_ imagIn, float* BV_R_ realOut) override
    {
        vecops::convert (m_a, realIn, m_half + 1);
        vecops::convert (m_b, imagIn, m_half + 1);
        transformI (m_a, m_b, realOut);
    }
    
    void inverseInterleaved (const double* BV_R_ complexIn, double* BV_R_ realOut) override
    {
        vecops::deinterleave (m_a_and_b, complexIn, 2, m_half + 1);
        transformI (m_a, m_b, realOut);
    }
    
    void inverseInterleaved (const float* BV_R_ complexIn, float* BV_R_ realOut) override
    {
        for (int i = 0; i <= m_half; ++i) m_a[i] = complexIn[i*2];
        for (int i = 0; i <= m_half; ++i) m_b[i] = complexIn[i*2+1];
        transformI (m_a, m_b, realOut);
    }
    
    void inversePolar (const double* BV_R_ magIn, const double* BV_R_ phaseIn, double* BV_R_ realOut) override
    {
        vecops::polar_to_cartesian (m_a, m_b, magIn, phaseIn, m_half + 1);
        transformI (m_a, m_b, realOut);
    }
    
    void inversePolar (const float* BV_R_ magIn, const float* BV_R_ phaseIn, float* BV_R_ realOut) override
    {
        vecops::convert (m_e, magIn, m_half + 1);
        vecops::convert (m_f, phaseIn, m_half + 1);
        vecops::polar_to_cartesian (m_a, m_b, m_e, m_f, m_half + 1);
        transformI (m_a, m_b, realOut);
    }
    
    void inverseCepstral (const double* BV_R_ magIn, double* BV_R_ cepOut) override
    {
        for (int i = 0; i <= m_half; ++i)
        {
            m_a[i] = log(magIn[i] + 0.000001);
            m_b[i] = 0.0;
        }
        transformI (m_a, m_b, cepOut);
    }
    
    void inverseCepstral (const float* BV_R_ magIn, float* BV_R_ cepOut) override
    {
        for (int i = 0; i <= m_half; ++i)
        {
            m_a[i] = double(logf(magIn[i] + 0.000001f));
            m_b[i] = 0.0;
        }
        transformI (m_a, m_b, cepOut);
    }
    
private:
    const int m_size;
    const int m_half;
    const int m_blockTableSize;
    const int m_maxTabledBlock;
    int *m_table;
    double *m_sincos;
    double *m_sincos_r;
    double *m_vr;
    double *m_vi;
    double *m_a;
    double *m_b;
    double *m_c;
    double *m_d;
    double *m_e;
    double *m_f;
    double *m_a_and_b[2];
    double *m_c_and_d[2];
    
    inline void makeTables()
    {
        // main table for complex fft - this is of size m_half
        
        int bits;
        int i, j, k, m;
        
        int n = m_half;
        
        for (i = 0; ; ++i) {
            if (n & (1 << i)) {
                bits = i;
                break;
            }
        }
        
        for (i = 0; i < n; ++i) {
            m = i;
            for (j = k = 0; j < bits; ++j) {
                k = (k << 1) | (m & 1);
                m >>= 1;
            }
            m_table[i] = k;
        }
        
        // sin and cos tables for complex fft
        int ix = 0;
        for (i = 2; i <= m_maxTabledBlock; i <<= 1)
        {
            auto phase = 2.0 * juce::MathConstants<double>::pi / double(i);
            m_sincos[ix++] = sin(phase);
            m_sincos[ix++] = sin(2.0 * phase);
            m_sincos[ix++] = cos(phase);
            m_sincos[ix++] = cos(2.0 * phase);
        }
        
        // sin and cos tables for real-complex transform
        ix = 0;
        for (i = 0; i < n/2; ++i) {
            double phase = juce::MathConstants<double>::pi * (double(i + 1) / double(m_half) + 0.5);
            m_sincos_r[ix++] = sin(phase);
            m_sincos_r[ix++] = cos(phase);
        }
    }
    
    // Uses m_a and m_b internally; does not touch m_c or m_d
    template <typename T>
    BV_FORCE_INLINE void transformF (const T* BV_R_ ri,
                                     double* BV_R_ ro, double* BV_R_ io)
    {
        
        int halfhalf = m_half / 2;
        for (int i = 0; i < m_half; ++i) {
            m_a[i] = ri[i * 2];
            m_b[i] = ri[i * 2 + 1];
        }
        transformComplex (m_a, m_b, m_vr, m_vi, false);
        ro[0] = m_vr[0] + m_vi[0];
        ro[m_half] = m_vr[0] - m_vi[0];
        io[0] = io[m_half] = 0.0;
        int ix = 0;
        for (int i = 0; i < halfhalf; ++i)
        {
            double s = -m_sincos_r[ix++];
            double c =  m_sincos_r[ix++];
            int k = i + 1;
            double r0 = m_vr[k];
            double i0 = m_vi[k];
            double r1 = m_vr[m_half - k];
            double i1 = -m_vi[m_half - k];
            double tw_r = (r0 - r1) * c - (i0 - i1) * s;
            double tw_i = (r0 - r1) * s + (i0 - i1) * c;
            ro[k] = (r0 + r1 + tw_r) * 0.5;
            ro[m_half - k] = (r0 + r1 - tw_r) * 0.5;
            io[k] = (i0 + i1 + tw_i) * 0.5;
            io[m_half - k] = (tw_i - i0 - i1) * 0.5;
        }
    }
    
    // Uses m_c and m_d internally; does not touch m_a or m_b
    template <typename T>
    BV_FORCE_INLINE void transformI (const double* BV_R_ ri, const double* BV_R_ ii,
                                     T* BV_R_ ro)
    {
        
        int halfhalf = m_half / 2;
        m_vr[0] = ri[0] + ri[m_half];
        m_vi[0] = ri[0] - ri[m_half];
        int ix = 0;
        for (int i = 0; i < halfhalf; ++i)
        {
            double s = m_sincos_r[ix++];
            double c = m_sincos_r[ix++];
            int k = i + 1;
            double r0 = ri[k];
            double r1 = ri[m_half - k];
            double i0 = ii[k];
            double i1 = -ii[m_half - k];
            double tw_r = (r0 - r1) * c - (i0 - i1) * s;
            double tw_i = (r0 - r1) * s + (i0 - i1) * c;
            m_vr[k] = (r0 + r1 + tw_r);
            m_vr[m_half - k] = (r0 + r1 - tw_r);
            m_vi[k] = (i0 + i1 + tw_i);
            m_vi[m_half - k] = (tw_i - i0 - i1);
        }
        transformComplex (m_vr, m_vi, m_c, m_d, true);
        for (int i = 0; i < m_half; ++i) {
            ro[i*2] = T(m_c[i]);
            ro[i*2+1] = T(m_d[i]);
        }
    }
    
    BV_FORCE_INLINE void transformComplex (const double* BV_R_ ri, const double* BV_R_ ii,
                                           double* BV_R_ ro, double* BV_R_ io,
                                           bool inverse)
    {
        
        // Following Don Cross's 1998 implementation, described by its
        // author as public domain.
        
        // Because we are at heart a real-complex fft only, and we know that:
        const int n = m_half;
        
        for (int i = 0; i < n; ++i) {
            int j = m_table[i];
            ro[j] = ri[i];
            io[j] = ii[i];
        }
        
        int ix = 0;
        int blockEnd = 1;
        double ifactor = (inverse ? -1.0 : 1.0);
        
        for (int blockSize = 2; blockSize <= n; blockSize <<= 1) {
            
            double sm1, sm2, cm1, cm2;
            
            if (blockSize <= m_maxTabledBlock) {
                sm1 = ifactor * m_sincos[ix++];
                sm2 = ifactor * m_sincos[ix++];
                cm1 = m_sincos[ix++];
                cm2 = m_sincos[ix++];
            } else {
                double phase = 2.0 * juce::MathConstants<double>::pi / double(blockSize);
                sm1 = ifactor * sin(phase);
                sm2 = ifactor * sin(2.0 * phase);
                cm1 = cos(phase);
                cm2 = cos(2.0 * phase);
            }
            
            double w = 2 * cm1;
            double ar[3], ai[3];
            
            for (int i = 0; i < n; i += blockSize) {
                
                ar[2] = cm2;
                ar[1] = cm1;
                
                ai[2] = sm2;
                ai[1] = sm1;
                
                int j = i;
                
                for (int m = 0; m < blockEnd; ++m) {
                    
                    ar[0] = w * ar[1] - ar[2];
                    ar[2] = ar[1];
                    ar[1] = ar[0];
                    
                    ai[0] = w * ai[1] - ai[2];
                    ai[2] = ai[1];
                    ai[1] = ai[0];
                    
                    int k = j + blockEnd;
                    double tr = ar[0] * ro[k] - ai[0] * io[k];
                    double ti = ar[0] * io[k] + ai[0] * ro[k];
                    
                    ro[k] = ro[j] - tr;
                    io[k] = io[j] - ti;
                    
                    ro[j] += tr;
                    io[j] += ti;
                    
                    ++j;
                }
            }
            
            blockEnd = blockSize;
        }
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FFT)
};


}  // namespace
