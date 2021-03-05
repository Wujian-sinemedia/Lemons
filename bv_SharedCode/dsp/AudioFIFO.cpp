
#include "bv_SharedCode/bv_SharedCode.h"


namespace bav
{
    
namespace dsp
{
    
    template<typename SampleType>
    void AudioFIFO<SampleType>::initialize (const int numChannels, const int size)
    {
        writeIndex = 0;
        storedSamples = 0;
        
        base.setSize (numChannels, size);
        
        constexpr SampleType zero = SampleType(0.0);
        
        for (int chan = 0; chan < numChannels; ++chan)
            juce::FloatVectorOperations::fill (base.getWritePointer(chan), zero, size);
    }
    
    template<typename SampleType>
    void AudioFIFO<SampleType>::releaseResources()
    {
        base.setSize (0, 0);
        writeIndex = 0;
        storedSamples = 0;
    }
    
    template<typename SampleType>
    void AudioFIFO<SampleType>::changeSize (const int newNumChannels, const int newSize)
    {
        if (base.getNumSamples() == newSize && base.getNumChannels() == newNumChannels)
            return;
        
        const juce::ScopedLock sl (lock);
        
        base.setSize (newNumChannels, newSize, true, true, true);
        
        if (writeIndex > newSize)
            writeIndex -= newSize;
        
        if (storedSamples > newSize)
            storedSamples = newSize;
    }
    
    template<typename SampleType>
    void AudioFIFO<SampleType>::pushSamples (const SampleType* inputSamples, const int numSamples, const int destChannel)
    {
        const juce::ScopedLock sl (lock);
        
        const int length = base.getNumSamples();
        
        jassert (length > 0 && base.getNumChannels() > 0);
        jassert (numSamples <= length);
        
        SampleType* writing = base.getWritePointer(destChannel);
        
        int index = writeIndex;
        
        for (int s = 0; s < numSamples; ++s, ++index)
        {
            if (index >= length) index = 0;
            writing[index] = inputSamples[s];
        }
        
        writeIndex = index;
        storedSamples += numSamples;
    }
    
    template<typename SampleType>
    void AudioFIFO<SampleType>::popSamples (SampleType* output, const int numSamples, const int readingChannel)
    {
        const juce::ScopedLock sl (lock);
        
        const int length = base.getNumSamples();
        
        jassert (length > 0 && base.getNumChannels() > 0);
        
        int readIndex = writeIndex - storedSamples;
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
        
        storedSamples -= numSamples;
        if (storedSamples < 0) storedSamples = 0;
    }
    
    template class AudioFIFO<float>;
    template class AudioFIFO<double>;
    
}  // namspace dsp
    
}  // namespace bav
