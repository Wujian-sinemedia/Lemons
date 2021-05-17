
namespace bav::dsp::FX
{

template<typename SampleType, size_t channels = 1>
class SmoothedGain
{
    using Smoother = juce::SmoothedValue<SampleType, juce::ValueSmoothingTypes::Multiplicative>;
    
public:
    SmoothedGain()
    {
        for (size_t i = 0; i < channels; ++i)
            smoothers.emplace_back (new Smoother);
    }
    
    virtual ~SmoothedGain() = default;
    
    void setGain (float gain)
    {
        const auto newTarget = static_cast<SampleType> (std::max (gain, 0.0001f));
        
        for (auto& smoother : smoothers)
            smoother.setTargetValue (newTarget);
    }
    
    void prepare (int blocksize)
    {
        lastBlocksize = blocksize;
        
        for (auto& smoother : smoothers)
            smoother.reset (blocksize);
    }
    
    void skipSamples (int numSamples)
    {
        for (auto& smoother : smoothers)
            for (int s = 0; s < numSamples; ++s)
                smoother.getNextValue();
    }
    
    void reset()
    {
        for (auto& smoother : smoothers)
            smoother.reset (lastBlocksize);
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
    std::vector< Smoother > smoothers;
    
    int lastBlocksize = 0;
};

template class SmoothedGain<float,  1>;
template class SmoothedGain<double, 1>;
template class SmoothedGain<float,  2>;
template class SmoothedGain<double, 2>;



/*
 A version that implements the ReorderableEffect interface, for use with my ReorderableFxChain class.
 */
template<typename SampleType>
class ReorderableSmoothedGain :     public SmoothedGain<SampleType>,
                                    public ReorderableEffect<SampleType>
{
public:
    ReorderableSmoothedGain() { }
    
protected:
    void fxChain_process (juce::AudioBuffer<SampleType>& audio) override
    {
        SmoothedGain<SampleType>::process (audio);
    }
    
    void fxChain_prepare (double, int blocksize) override
    {
        SmoothedGain<SampleType>::prepare (blocksize);
    }
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReorderableSmoothedGain)
};

template class ReorderableSmoothedGain<float>;
template class ReorderableSmoothedGain<double>;


}  // namespace
