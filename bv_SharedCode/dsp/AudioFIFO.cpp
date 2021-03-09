
#include "bv_SharedCode/bv_SharedCode.h"


namespace bav
{
    
namespace dsp
{

template<typename SampleType>
void AudioFIFO<SampleType>::initialize (const int numChannels, const int size)
{
    writeIndex = 0;
    
    base.setSize (numChannels, size * 2);
    storedSamples.ensureStorageAllocated (numChannels);
    
    constexpr SampleType zero = SampleType(0.0);
    
    for (int chan = 0; chan < numChannels; ++chan)
    {
        juce::FloatVectorOperations::fill (base.getWritePointer(chan), zero, size);
        storedSamples.set (chan, 0);
    }
}

    
template<typename SampleType>
void AudioFIFO<SampleType>::releaseResources()
{
    base.setSize (0, 0);
    storedSamples.clear();
    writeIndex = 0;
    storedSamples = 0;
}

    
template<typename SampleType>
void AudioFIFO<SampleType>::changeSize (const int newNumChannels, int newSize)
{
    newSize += newSize;
    
    if (base.getNumSamples() == newSize && base.getNumChannels() == newNumChannels)
        return;
    
    base.setSize (newNumChannels, newSize, true, true, true);
    storedSamples.ensureStorageAllocated (newNumChannels);
    
    if (writeIndex > newSize)
        writeIndex -= newSize;
    
    for (int chan = 0; chan < newNumChannels; ++chan)
    {
        const int prev = storedSamples.getUnchecked(chan);
        
        if (prev > newSize)
            storedSamples.set (chan, newSize);
    }
}

    
template<typename SampleType>
void AudioFIFO<SampleType>::pushSamples (const SampleType* inputSamples, const int numSamples, const int destChannel)
{
    const int length = base.getNumSamples();
    
    jassert (length > 0 && base.getNumChannels() > 0);
    
    int ns = storedSamples.getUnchecked(destChannel);
    
    jassert (numSamples + ns <= length);
    
    SampleType* writing = base.getWritePointer(destChannel);
    
    int index = writeIndex;
    
    for (int s = 0; s < numSamples; ++s, ++index)
    {
        if (index >= length) index = 0;
        writing[index] = inputSamples[s];
    }
    
    writeIndex = index;
    
    ns += numSamples;
    if (ns > length) ns = length;
    storedSamples.set (destChannel, ns);
}
    

template<typename SampleType>
void AudioFIFO<SampleType>::popSamples (SampleType* output, const int numSamples, const int readingChannel)
{
    const int length = base.getNumSamples();
    
    jassert (length > 0 && base.getNumChannels() > 0);
    
    int ns = storedSamples.getUnchecked(readingChannel);
    
    int readIndex = writeIndex - ns;
    if (readIndex < 0) readIndex = length - writeIndex;
    
    jassert (readIndex >= 0 && readIndex < length);
    
    const SampleType* reading = base.getReadPointer(readingChannel);
    SampleType* writing = base.getWritePointer(readingChannel);
    
    constexpr SampleType zero = SampleType(0.0);
    
    for (int s = 0; s < numSamples; ++s, ++readIndex)
    {
        if (readIndex >= length) readIndex = 0;
        output[s] = reading[readIndex];
        writing[s] = zero;
    }
    
    ns -= numSamples;
    if (ns < 0) ns = 0;
    storedSamples.set (readingChannel, ns);
}

    
template class AudioFIFO<float>;
template class AudioFIFO<double>;

    
}  // namspace dsp

}  // namespace bav

