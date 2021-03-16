
namespace bav
{

namespace dsp
{
    
    template<typename SampleType>
    class SidechainedLimiter
    {
    public:
        SidechainedLimiter() { }
        
        ~SidechainedLimiter() { }
        
        void setThreshold (float thresh_dB)
        {
            
        }
        
        void setRelease (float release_ms)
        {
            
        }
        
        void prepare (int blocksize, double samplerate, int numChannels)
        {
            
        }
        
        void reset()
        {
            
        }
        
        
        void process (const juce::AudioBuffer<SampleType>& sidechain,
                      juce::AudioBuffer<SampleType>& signalToLimit)
        {
            const int numChannels = signalToLimit.getNumChannels();
            const int numSamples  = signalToLimit.getNumSamples();
            
            jassert (sidechain.getNumChannels() == numChannels);
            jassert (sidechain.getNumSamples() == numSamples);
            
            for (int channel = 0; channel < numChannels; ++channel)
            {
                process (channel,
                         sidechain.getReadPointer (channel),
                         signalToLimit.getWritePointer (channel),
                         numSamples);
            }
        }
        
        
        void process (const SampleType* sidechain,
                      SampleType* signalToLimit,
                      int numSamples)
        {
            
        }
        
        
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SidechainedLimiter)
    };
    
    
    template class SidechainedLimiter<float>;
    template class SidechainedLimiter<double>;
    
}  // namespace dsp

}  // namespace bav
