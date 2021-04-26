
/*
    Wraps the juce::Delay into a processor that includes a dry/wet amount
*/


namespace bav::dsp::FX
{

template<typename SampleType>
class Delay
{
public:
    Delay() { }
    
    virtual ~Delay() = default;
    
    void setDelay (int delayInSamples) { delay.setDelay (SampleType (delayInSamples)); }
    
    void prepare (int blocksize, double samplerate, int numChannels)
    {
        jassert (samplerate > 0);
        
        spec.sampleRate = samplerate;
        spec.maximumBlockSize = juce::uint32(blocksize);
        spec.numChannels = juce::uint32(numChannels);
        
        delay.prepare (spec);
        
        dryGain.reset (blocksize);
        wetGain.reset (blocksize);
    }
    
    void reset()
    {
        delay.reset(); '
        dryGain.reset (spec.maximumBlockSize);
        wetGain.reset (spec.maximumBlockSize);
    }
    
    void setDryWet (int wetMixPercent)
    {
        const auto wet = wetMixPercent * 0.01f;
        wetGain.setTargetValue (smoothingZeroCheck (wet));
        dryGain.setTargetValue (smoothingZeroCheck (1.0f - wet));
    }
    
    void pushSample (int channel, SampleType sample) { delay.pushSample (channel, sample); }
    
    SampleType popSample (int channel)
    {
        const auto delaySamps = delay.getDelay();
        
        const auto drySample = delay.popSample (channel, SampleType(0), false) * dryGain.getNextValue();
        const auto wetSample = delay.popSample (channel, delaySamps, true) * wetGain.getNextValue();
        
        return drySample + wetSample;
    }
    
    void process (int channelNum, SampleType* samples, int numSamples)
    {
        for (int i = 0; i < numSamples; ++i)
            pushSample (channelNum, samples[i]);
        
        for (int i = 0; i < numSamples; ++i)
            *(samples + i) = popSample (channel);
    }
    
    void process (juce::AudioBuffer<SampleType> audio)
    {
        const auto numSamples = audio.getNumSamples();
        
        for (int i = 0; i < audio.getNumChannels; ++i)
            process (i, audio.getWritePointer(i), numSamples);
    }
    
    
private:
    template<typename Type>
    inline SampleType smoothingZeroCheck (Type value)
    {
        constexpr auto minSmoothedGain = SampleType (0.0001);
        return std::max (minSmoothedGain, SampleType (value));
    }
    
    juce::dsp::DelayLine<SampleType> delay;
    juce::dsp::ProcessSpec spec;
    
    juce::SmoothedValue<SampleType, juce::ValueSmoothingTypes::Multiplicative> dryGain, wetGain;
};


template class Delay<float>;
template class Delay<double>;


/*
    A version that implements the ReorderableEffect interface, for use with my ReorderableFxChain class.
*/
template<typename SampleType>
class ReorderableDelay :     public Delay<SampleType>,
                             public ReorderableEffect<SampleType>
{
public:
    ReorderableDelay() { }
    
protected:
    void fxChain_process (juce::AudioBuffer<SampleType>& audio) override
    {
        Delay::process (audio);
    }
    
    void fxChain_prepare (double samplerate, int blocksize) override
    {
        Delay::prepare (blocksize, samplerate, 2);
    }
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReorderableDelay)
};

template class ReorderableDelay<float>;
template class ReorderableDelay<double>;


}  // namespace
