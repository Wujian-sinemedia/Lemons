/*
 Part of module: bv_SharedCode
 Parent file: bv_SharedCode.h
 */


namespace bav::dsp
{
    
    
/*
    simple FIFO that uses a juce::AudioBuffer as its base storage.
 
    This class is NOT thread-safe, it is intended to be written to and read from by the same thread.
*/

template<typename SampleType>
class AudioFIFO
{
public:
    
    AudioFIFO (const int numChannels, const int size);
    
    AudioFIFO();
    
    virtual ~AudioFIFO() = default;
    
    
    void initialize (const int numChannels, const int size);
    
    void clear();
    
    void releaseResources();
    
    void changeSize (const int newNumChannels, int newSize);
    
    
    void pushSamples (const juce::AudioBuffer<SampleType>& inputBuffer, const int inputChannel,
                      const int inputStartSample, const int numSamples,
                      const int destChannel);
    
    
    void pushSamples (const SampleType* inputSamples, const int numSamples, const int destChannel);
    
    
    void popSamples (juce::AudioBuffer<SampleType>& destBuffer, const int destChannel,
                     const int destStartSample, const int numSamples,
                     const int readingChannel);
    
    
    void popSamples (SampleType* output, const int numSamples, const int readingChannel);
    
    
    int getSize() const noexcept { return base.getNumSamples(); }
    
    int getNumChannels() const noexcept { return base.getNumChannels(); }
    
    // returns the number of samples stored in a particular channel of the base buffer
    int numStoredSamples (int channel) const { return storedSamples.getUnchecked (channel); }
    
    /* returns the lowest number of samples stored in any channel
     (essentially, the highest sample index you can safely access in a for loop iterating over all channels of this FIFO) */
    int numStoredSamples() const;
    
    
private:
    
    juce::AudioBuffer<SampleType> base;
    
    // need to store the write index & number of stored samples for EACH channel!
    juce::Array<int> writeIndex;
    juce::Array<int> storedSamples;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioFIFO)
};

    
} // namespace bav



