
#include "bv_SharedCode/bv_SharedCode.h"


namespace bav
{

namespace dsp
{

template<typename SampleType>
void AudioFIFO<SampleType>::initialize (const int numChannels, const int size)
{
    base.setSize (numChannels, size * 2);
    storedSamples.ensureStorageAllocated (numChannels);
    
    constexpr SampleType zero = SampleType(0.0);
    
    for (int chan = 0; chan < numChannels; ++chan)
    {
        juce::FloatVectorOperations::fill (base.getWritePointer(chan), zero, base.getNumSamples());
        storedSamples.set (chan, 0);
        writeIndex.set (chan, 0);
    }
}


template<typename SampleType>
void AudioFIFO<SampleType>::releaseResources()
{
    base.setSize (0, 0);
    storedSamples.clear();
    writeIndex.clear();
}
    

template<typename SampleType>
void AudioFIFO<SampleType>::clear()
{
    base.clear();
    
    for (int i = 0; i < base.getNumChannels(); ++i)
    {
        storedSamples.set (i, 0);
        writeIndex.set (1, 0);
    }
}

    
template<typename SampleType>
int AudioFIFO<SampleType>::numStoredSamples() const
{
    jassert (! storedSamples.isEmpty());
    
    int minNumSamples = storedSamples.getUnchecked(0);
    
    for (int stored : storedSamples)
        if (stored < minNumSamples)
            minNumSamples = stored;
    
    return stored;
}


template<typename SampleType>
void AudioFIFO<SampleType>::changeSize (const int newNumChannels, int newSize)
{
    newSize += newSize;
    
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
            const int prev = storedSamples.getUnchecked(chan);
            
            if (prev > newSize)
                storedSamples.set (chan, newSize);
        }
        
        if (writeIndex.isEmpty() || chan >= writeIndex.size())
        {
            writeIndex.add (0);
        }
        else
        {
            const int prevW = writeIndex.getUnchecked(chan);
            
            if (prevW > newSize)
                writeIndex.set (chan, newSize);
        }
    }
}


template<typename SampleType>
void AudioFIFO<SampleType>::pushSamples (const SampleType* inputSamples, const int numSamples, const int destChannel)
{
    jassert (destChannel >= 0 && destChannel < base.getNumChannels());
    
    const int length = base.getNumSamples();
    
    jassert (length > 0 && base.getNumChannels() > 0);
    
    int ns = storedSamples.getUnchecked(destChannel);
    
    jassert (numSamples + ns <= length);
    
    SampleType* writing = base.getWritePointer(destChannel);
    
    int index = writeIndex.getUnchecked(destChannel);
    
    for (int s = 0; s < numSamples; ++s, ++index)
    {
        if (index >= length) index = 0;
        writing[index] = inputSamples[s];
    }
    
    writeIndex.set (destChannel, index);
    storedSamples.set (destChannel, ns += numSamples);
}


template<typename SampleType>
void AudioFIFO<SampleType>::popSamples (SampleType* output, const int numSamples, const int readingChannel)
{
    jassert (readingChannel >= 0 && readingChannel < base.getNumChannels());
    
    const int length = base.getNumSamples();
    
    jassert (length > 0 && base.getNumChannels() > 0);
    
    int ns = storedSamples.getUnchecked(readingChannel);
    
    int readIndex = writeIndex.getUnchecked(readingChannel) - ns;
    if (readIndex < 0) readIndex += length;
    
    jassert (readIndex >= 0 && readIndex < length);
    
    const SampleType* reading = base.getReadPointer(readingChannel);
    SampleType* writing = base.getWritePointer(readingChannel);
    
    constexpr SampleType zero = SampleType(0.0);
    
    for (int s = 0, index = readIndex; s < numSamples; ++s, ++index)
    {
        if (index >= length) index = 0;
        output[s] = reading[index];
        writing[index] = zero;
    }
    
    storedSamples.set (readingChannel, std::max (0, ns - numSamples));
}


template class AudioFIFO<float>;
template class AudioFIFO<double>;


}  // namspace dsp

}  // namespace bav


