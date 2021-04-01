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
    
    AudioFIFO (const int numChannels, const int size)
    {
        initialize (numChannels, size);
    }
    
    AudioFIFO()
    {
        writeIndex.add(0);
        storedSamples.add(0);
        base.setSize (0, 0);
    }
    
    ~AudioFIFO()
    { }
    
    
    void initialize (const int numChannels, const int size)
    {
        base.setSize (numChannels, size * 2);
        storedSamples.ensureStorageAllocated (numChannels);
        
        constexpr auto zero = SampleType(0.0);
        
        for (int chan = 0; chan < numChannels; ++chan)
        {
            juce::FloatVectorOperations::fill (base.getWritePointer(chan), zero, base.getNumSamples());
            storedSamples.set (chan, 0);
            writeIndex.set (chan, 0);
        }
    }
    
    
    void clear()
    {
        base.clear();
        
        for (int i = 0; i < base.getNumChannels(); ++i)
        {
            storedSamples.set (i, 0);
            writeIndex.set (1, 0);
        }
    }
    
    
    void releaseResources()
    {
        base.setSize (0, 0);
        storedSamples.clear();
        writeIndex.clear();
    }
    
    
    void changeSize (const int newNumChannels, int newSize)
    {
        newSize += newSize;  // the real size is always blocksize * 2 under the hood
        
        if (base.getNumSamples() == newSize && base.getNumChannels() == newNumChannels)
            return;
        
        base.setSize (newNumChannels, newSize, true, true, true);
        storedSamples.ensureStorageAllocated (newNumChannels);
        
        for (int chan = 0; chan < newNumChannels; ++chan)
        {
            if (storedSamples.isEmpty() || chan >= storedSamples.size())
            {
                storedSamples.add (0);
            }
            else
            {
                const auto prev = storedSamples.getUnchecked(chan);
                
                if (prev > newSize)
                    storedSamples.set (chan, newSize);
            }
            
            if (writeIndex.isEmpty() || chan >= writeIndex.size())
            {
                writeIndex.add (0);
            }
            else
            {
                const auto prevW = writeIndex.getUnchecked(chan);
                
                if (prevW > newSize)
                    writeIndex.set (chan, newSize);
            }
        }
    }
    
    
    void pushSamples (const juce::AudioBuffer<SampleType>& inputBuffer, const int inputChannel,
                      const int inputStartSample, const int numSamples,
                      const int destChannel)
    {
        pushSamples (inputBuffer.getReadPointer(inputChannel) + inputStartSample, numSamples, destChannel);
    }
    
    
    void pushSamples (const SampleType* inputSamples, const int numSamples, const int destChannel)
    {
        jassert (destChannel >= 0 && destChannel < base.getNumChannels());
        
        const auto length = base.getNumSamples();
        
        jassert (length > 0 && base.getNumChannels() > 0);
        jassert (numSamples + storedSamples.getUnchecked(destChannel) <= length);
        
        auto* writing = base.getWritePointer(destChannel);
        
        auto index = writeIndex.getUnchecked(destChannel);
        
        for (int s = 0; s < numSamples; ++s, ++index)
        {
            if (index >= length) index = 0;
            writing[index] = inputSamples[s];
        }
        
        writeIndex.set (destChannel, index);
        storedSamples.set (destChannel, storedSamples.getUnchecked(destChannel) + numSamples);
    }
    
    
    void popSamples (juce::AudioBuffer<SampleType>& destBuffer, const int destChannel,
                     const int destStartSample, const int numSamples,
                     const int readingChannel)
    {
        popSamples (destBuffer.getWritePointer(destChannel) + destStartSample, numSamples, readingChannel);
    }
    
    
    void popSamples (SampleType* output, const int numSamples, const int readingChannel)
    {
        jassert (readingChannel >= 0 && readingChannel < base.getNumChannels());
        
        const auto length = base.getNumSamples();
        
        jassert (length > 0 && base.getNumChannels() > 0);
        
        const auto ns = storedSamples.getUnchecked(readingChannel);
        
        auto readIndex = writeIndex.getUnchecked(readingChannel) - ns;
        if (readIndex < 0) readIndex += length;
        
        jassert (readIndex >= 0 && readIndex <= length);
        
        const auto* reading = base.getReadPointer(readingChannel);
        auto* writing = base.getWritePointer(readingChannel);
        
        constexpr auto zero = SampleType(0.0);
        
        for (int s = 0, index = readIndex; s < numSamples; ++s, ++index)
        {
            if (index >= length) index = 0;
            output[s] = reading[index];
            writing[index] = zero;
        }
        
        storedSamples.set (readingChannel, std::max (0, ns - numSamples));
    }
    
    
    int getSize() const noexcept { return base.getNumSamples(); }
    
    
    // returns the number of samples stored in a particular channel of the base buffer
    int numStoredSamples(int channel) const { return storedSamples.getUnchecked(channel); }
    
    /* returns the lowest number of samples stored in any channel
     (essentially, the highest sample index you can safely access in a for loop iterating over all channels of this FIFO) */
    int numStoredSamples() const
    {
        jassert (! storedSamples.isEmpty());
        
        auto minNumSamples = storedSamples.getUnchecked(0);
        
        for (int stored : storedSamples)
            if (stored < minNumSamples)
                minNumSamples = stored;
        
        return minNumSamples;
    }
    
    
private:
    
    juce::AudioBuffer<SampleType> base;
    
    // need to store the write index & number of stored samples for EACH channel!
    juce::Array<int> writeIndex;
    juce::Array<int> storedSamples;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioFIFO)
};


    
template class AudioFIFO<float>;
template class AudioFIFO<double>;

    
} // namespace bav



