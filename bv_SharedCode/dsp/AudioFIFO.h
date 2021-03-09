/*
 Part of module: bv_SharedCode
 Parent file: bv_SharedCode.h
 */


namespace bav

{

namespace dsp
{
    

template<typename SampleType>
class AudioFIFO
{
public:
    
    AudioFIFO (const int numChannels, const int size)
    {
        initialize (numChannels, size);
    }
    
    AudioFIFO()
    {
        writeIndex = 0;
        storedSamples.add(0);
        base.setSize (0, 0);
    }
    
    ~AudioFIFO()
    { }
    
    
    void initialize (const int numChannels, const int size);
    
    
    void releaseResources();
    
    
    void changeSize (const int newNumChannels, const int newSize);
    
    
    void pushSamples (const juce::AudioBuffer<SampleType>& inputBuffer, const int inputChannel,
                      const int inputStartSample, const int numSamples,
                      const int destChannel)
    {
        pushSamples (inputBuffer.getReadPointer(inputChannel, inputStartSample),
                     numSamples, destChannel);
    }
    
    
    void pushSamples (const SampleType* inputSamples, const int numSamples, const int destChannel);
    
    
    void popSamples (juce::AudioBuffer<SampleType>& destBuffer, const int destChannel,
                     const int destStartSample, const int numSamples,
                     const int readingChannel)
    {
        popSamples (destBuffer.getWritePointer(destChannel, destStartSample),
                    numSamples, readingChannel);
    }
    
    
    void popSamples (SampleType* output, const int numSamples, const int readingChannel);
    
    
    int numStoredSamples(int channel) const noexcept { return storedSamples.getUnchecked(channel); }
    
    
    int getSize() const noexcept { return base.getNumSamples(); }
    
    
private:
    
    juce::AudioBuffer<SampleType> base;
    
    int writeIndex;
    
    juce::Array<int> storedSamples;  // need to store the # of samples in EACH channel!
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioFIFO)
};


}  // namespace dsp

} // namespace bav


