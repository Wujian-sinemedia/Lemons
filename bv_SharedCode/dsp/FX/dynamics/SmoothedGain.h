
namespace bav::dsp::FX
{

template<typename SampleType, size_t channels = 1>
class SmoothedGain
{
public:
    SmoothedGain() = default;
    
    virtual ~SmoothedGain() = default;
    
    void setGain (float gain)
    {
        const auto newTarget = static_cast<SampleType> (std::max (gain, 0.0001f));
        
        for (size_t i = 0; i < channels; ++i)
            smoothers[i].setTargetValue (newTarget);
    }
    
    void prepare (int blocksize)
    {
        lastBlocksize = blocksize;
        
        for (size_t i = 0; i < channels; ++i)
            smoothers[i].reset (blocksize);
    }
    
    void skipSamples (int numSamples)
    {
        for (size_t i = 0; i < channels; ++i)
            for (int s = 0; s < numSamples; ++s)
                smoothers[i].getNextValue();
    }
    
    void reset()
    {
        for (size_t i = 0; i < channels; ++i)
            smoothers[i].reset (lastBlocksize);
    }
    
    
    void process (SampleType* samples, int numSamples, int channel, float newGain)
    {
        setGain (newGain);
        process (samples, numSamples, channel);
    }
    
    void process (SampleType* samples, int numSamples, int channel)
    {
        jassert (channel < channels);
        smoothers[channel].applyGain (samples, numSamples);
    }
    
    void process (juce::AudioBuffer<SampleType>& audio, float newGain)
    {
        setGain (newGain);
        process (audio);
    }
    
    void process (juce::AudioBuffer<SampleType>& audio)
    {
        const auto numSamples = audio.getNumSamples();
        
        for (size_t chan = 0; chan < std::min (channels, audio.getNumChannels()); ++chan)
            process (audio.getWritePointer (chan), numSamples, chan);
    }
    
    
private:
    juce::SmoothedValue<SampleType, juce::ValueSmoothingTypes::Multiplicative>[channels] smoothers;
    
    int lastBlocksize = 0;
};

template class SmoothedGain<float,  1>;
template class SmoothedGain<double, 1>;
template class SmoothedGain<float,  2>;
template class SmoothedGain<double, 2>;


}  // namespace
