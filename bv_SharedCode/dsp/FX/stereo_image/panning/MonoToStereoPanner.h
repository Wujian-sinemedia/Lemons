
namespace bav::dsp::FX
{

template<typename SampleType>
class MonoToStereoPanner  :     public PannerBase
{
public:
    MonoToStereoPanner() { }
    
    virtual ~MonoToStereoPanner() = default;
    
    
    void prepare (int blocksize)
    {
        lastBlocksize = blocksize;
        reset();
    }
    
    void reset()
    {
        left.reset  (lastBlocksize);
        right.reset (lastBlocksize);
    }
    
    
    void process (const SampleType* input, SampleType* leftOut, SampleType* rightOut, int numSamples)
    {
        left.setTargetValue  (smoothingZeroCheck (PannerBase::getLeftGain()));
        right.setTargetValue (smoothingZeroCheck (PannerBase::getRightGain()));
        
        for (int i = 0; i < numSamples; ++i)
        {
            const auto sample = input[i];
            leftOut[i]  = sample * left.getNextValue();
            rightOut[i] = sample * right.getNextValue();
        }
    }
    
    
    void process (const juce::AudioBuffer<SampleType>& monoInput,
                  juce::AudioBuffer<SampleType>& stereoOutput)
    {
        stereoOutput.clear();
        jassert (stereoOutput.getNumChannels() >= 2);
        jassert (monoInput.getNumSamples() == stereoOutput.getNumSamples());
        
        process (monoInput.getReadPointer (0),
                 stereoOutput.getWritePointer (0),
                 stereoOutput.getWritePointer (1),
                 stereoOutput.getNumSamples());
    }
    
    
private:
    juce::SmoothedValue<SampleType, juce::ValueSmoothingTypes::Multiplicative> left, right;
    int lastBlocksize = 0;
    
    inline SampleType smoothingZeroCheck (float gain)
    {
        return static_cast<SampleType> (std::max (gain, 0.001f));
    }
};


template class MonoToStereoPanner<float>;
template class MonoToStereoPanner<double>;

}  // namespace
