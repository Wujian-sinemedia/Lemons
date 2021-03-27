
/*
    FFT class using Ne10.
*/

namespace bav::dsp
{
    
    class FFT :     public FFTinterface
    {
    public:
        FFT() { }
        
        ~FFT() override { }
        
        int getSize() const override { return 0; }
        
        void initFloat() override { }
        void initDouble() override { }
        
        bool isFloatInitialized() override { return false; }
        bool isDoubleInitialized() override { return false; }
        
        void forward (const double* BV_R_ realIn, double* BV_R_ realOut, double* BV_R_ imagOut) override
        {
            
        }
        
        void forward (const float* BV_R_ realIn, float* BV_R_ realOut, float* BV_R_ imagOut) override
        {
            
        }
        
        void forwardInterleaved (const double* BV_R_ realIn, double* BV_R_ complexOut) override
        {
            
        }
        
        void forwardInterleaved (const float* BV_R_ realIn, float* BV_R_ complexOut) override
        {
            
        }
        
        void forwardPolar (const double* BV_R_ realIn, double* BV_R_ magOut, double* BV_R_ phaseOut) override
        {
            
        }
        
        void forwardPolar (const float* BV_R_ realIn, float* BV_R_ magOut, float* BV_R_ phaseOut) override
        {
            
        }
        
        void forwardMagnitude (const double* BV_R_ realIn, double* BV_R_ magOut) override
        {
            
        }
        
        void forwardMagnitude (const float* BV_R_ realIn, float* BV_R_ magOut) override
        {
            
        }
        
        /*
        */
        
        void inverse (const double* BV_R_ realIn, const double* BV_R_ imagIn, double* BV_R_ realOut) override
        {
            
        }
        
        void inverse (const float* BV_R_ realIn, const float* BV_R_ imagIn, float* BV_R_ realOut) override
        {
            
        }
        
        void inverseInterleaved (const double* BV_R_ complexIn, double* BV_R_ realOut) override
        {
            
        }
        
        void inverseInterleaved (const float* BV_R_ complexIn, float* BV_R_ realOut) override
        {
            
        }
        
        void inversePolar (const double* BV_R_ magIn, const double* BV_R_ phaseIn, double* BV_R_ realOut) override
        {
            
        }
        
        void inversePolar (const float* BV_R_ magIn, const float* BV_R_ phaseIn, float* BV_R_ realOut) override
        {
            
        }
        
        void inverseCepstral (const double* BV_R_ magIn, double* BV_R_ cepOut) override
        {
            
        }
        
        void inverseCepstral (const float* BV_R_ magIn, float* BV_R_ cepOut) override
        {
            
        }
        
        /*
        */
        
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FFT)
    };
    
    
}  // namespace
