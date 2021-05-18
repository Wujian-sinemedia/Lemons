
namespace bav::dsp::FX
{

template<typename SampleType>
class StereoPanner  :     public PannerBase
{
public:
    StereoPanner() { }
    
    virtual ~StereoPanner() = default;
    
    
    void prepare (int blocksize)
    {
        left.prepare (blocksize);
        right.prepare (blocksize);
    }
    
    void reset()
    {
        left.reset();
        right.reset();
    }
    
    
    void process (const SampleType* leftIn, const SampleType* rightIn,
                  SampleType* leftOut, SampleType* rightOut, int numSamples)
    {
        left.setGain  (PannerBase::getLeftGain());
        right.setGain (PannerBase::getRightGain());
        
        vecops::copy (leftIn, leftOut,  numSamples);
        vecops::copy (rightIn, rightOut, numSamples);
        
        left.process  (leftOut,  numSamples, 0);
        right.process (rightOut, numSamples, 0);
    }
    
    
    void process (const juce::AudioBuffer<SampleType>& stereoInput,
                  juce::AudioBuffer<SampleType>& stereoOutput)
    {
        stereoOutput.clear();
        jassert (stereoInput.getNumChannels() >= 2 && stereoOutput.getNumChannels() >= 2);
        jassert (monoInput.getNumSamples() == stereoOutput.getNumSamples());
        
        process (stereoInput.getReadPointer (0), stereoInput.getReadPointer (1),
                 stereoOutput.getWritePointer (0), stereoOutput.getWritePointer (1),
                 stereoOutput.getNumSamples());
    }
    
    
private:
    SmoothedGain<SampleType, 1> left, right;
};


template class StereoPanner<float>;
template class StereoPanner<double>;


/*
 A version that implements the ReorderableEffect interface, for use with my ReorderableFxChain class.
 */
template<typename SampleType>
class ReorderableStereoPanner :    public StereoPanner<SampleType>,
                                   public ReorderableEffect<SampleType>
{
public:
    ReorderableStereoPanner() { }
    
protected:
    void fxChain_process (juce::AudioBuffer<SampleType>& audio) override
    {
        StereoPanner<SampleType>::process (audio, audio);
    }
    
    void fxChain_prepare (double, int) override
    {
        StereoPanner<SampleType>::prepare();
    }
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReorderableLimiter)
};

template class ReorderableStereoPanner<float>;
template class ReorderableStereoPanner<double>;


}  // namespace

