#include <lemons_core/lemons_core.h>

namespace lemons::dsp
{

template <typename SampleType>
CircularBuffer<SampleType>::CircularBuffer (int initialCapacity)
: buffer (1, initialCapacity), fifo (initialCapacity)
{
    
}

template <typename SampleType>
void CircularBuffer<SampleType>::storeSamples (const AudioBuffer<SampleType>& samples, int channel)
{
	jassert (channel >= 0);
	storeSamples (samples.getReadPointer (channel), samples.getNumSamples());
}

template <typename SampleType>
void CircularBuffer<SampleType>::storeSamples (const SampleType* samples, int numSamples)
{
    jassert (buffer.getNumSamples() == fifo.getTotalSize());
    
    const auto scopedWrite = fifo.write (numSamples);
    
    // the buffer isn't big enough to hold all the samples you want to store!
    // to avoid this assertion, you should always allocate the CircularBuffer's total size to be larger than the actual total number of samples you need to store.
    // even allocating blocksize + 1 can help to avoid this assertion.
    jassert (scopedWrite.blockSize1 + scopedWrite.blockSize2 == numSamples);
    
    if (scopedWrite.blockSize1 > 0)
    {
        vecops::copy (samples,
                      buffer.getWritePointer (0, scopedWrite.startIndex1),
                      scopedWrite.blockSize1);
    }
    
    if (scopedWrite.blockSize2 > 0)
    {
        vecops::copy (samples + scopedWrite.blockSize1,
                      buffer.getWritePointer (0, scopedWrite.startIndex2),
                      scopedWrite.blockSize2);
    }
}

template <typename SampleType>
void CircularBuffer<SampleType>::getSamples (AudioBuffer<SampleType>& output, int channel)
{
	jassert (channel >= 0);
	getSamples (output.getWritePointer (channel), output.getNumSamples());
}

template <typename SampleType>
void CircularBuffer<SampleType>::getSamples (SampleType* output, int numSamples)
{
    jassert (buffer.getNumSamples() == fifo.getTotalSize());
    
    const auto scopedRead = fifo.read (numSamples);
    
    const auto numZeroes = numSamples - (scopedRead.blockSize1 + scopedRead.blockSize2);
    
    if (numZeroes > 0)
        vecops::fill (output, SampleType(0), numZeroes);
    
    auto* const sampleOutput = output + numZeroes;
    
    if (scopedRead.blockSize1 > 0)
    {
        vecops::copy (buffer.getReadPointer (0, scopedRead.startIndex1),
                      sampleOutput,
                      scopedRead.blockSize1);
    }
    
    if (scopedRead.blockSize2 > 0)
    {
        vecops::copy (buffer.getReadPointer (0, scopedRead.startIndex2),
                      sampleOutput + scopedRead.blockSize1,
                      scopedRead.blockSize2);
    }
}

template <typename SampleType>
void CircularBuffer<SampleType>::resize (int newSize)
{
    clear();
    
    buffer.setSize (1, newSize, true, true, false);
    
    fifo.setTotalSize (newSize);
    
    jassert (buffer.getNumSamples() == fifo.getTotalSize());
}

template <typename SampleType>
int CircularBuffer<SampleType>::getCapacity() const noexcept
{
    jassert (buffer.getNumSamples() == fifo.getTotalSize());
	return buffer.getNumSamples();
}

template <typename SampleType>
int CircularBuffer<SampleType>::getNumStoredSamples() const noexcept
{
    jassert (buffer.getNumSamples() == fifo.getTotalSize());
    return fifo.getNumReady();
}

template <typename SampleType>
void CircularBuffer<SampleType>::clear()
{
	buffer.clear();
    fifo.reset();
}


template class CircularBuffer<float>;
template class CircularBuffer<double>;

}  // namespace lemons::dsp


/*---------------------------------------------------------------------------------------------------------------------------------*/


#if LEMONS_UNIT_TESTS

template<typename FloatType>
inline bool allSamplesAreEqual (const juce::AudioBuffer<FloatType>& buffer1,
                                const juce::AudioBuffer<FloatType>& buffer2,
                                int startIndex, int numSamples)
{
    jassert (startIndex + numSamples <= buffer1.getNumSamples());
    jassert (startIndex + numSamples <= buffer2.getNumSamples());
    
    for (int i = startIndex; i < startIndex + numSamples; ++i)
        if (buffer1.getSample (0, i) != buffer2.getSample (0, i))
            return false;
    
    return true;
}

template bool allSamplesAreEqual (const juce::AudioBuffer<float>&, const juce::AudioBuffer<float>&, int, int);
template bool allSamplesAreEqual (const juce::AudioBuffer<double>&, const juce::AudioBuffer<double>&, int, int);


template<typename FloatType>
inline bool allSamplesAreZero (const juce::AudioBuffer<FloatType>& buffer,
                               int startIndex, int numSamples)
{
    jassert (startIndex + numSamples <= buffer.getNumSamples());
    
    for (int i = startIndex; i < startIndex + numSamples; ++i)
        if (buffer.getSample (0, i) != FloatType(0))
            return false;
    
    return true;
}

template bool allSamplesAreZero (const juce::AudioBuffer<float>&, int, int);
template bool allSamplesAreZero (const juce::AudioBuffer<double>&, int, int);


CircularBufferTests::CircularBufferTests()
: juce::UnitTest ("CircularBufferTests", "DSP")
{
}

void CircularBufferTests::initialise()
{
    osc.setFrequency (440.f, 44100.);
}

void CircularBufferTests::resizeAllBuffers (int newSize)
{
    circularBuffer.resize (newSize + 1);
    
    origStorage.setSize (1, newSize);
    circOutput.setSize (1, newSize);
}

void CircularBufferTests::runTest()
{
    constexpr auto numSamples = 44100;
    
    resizeAllBuffers (numSamples);
    
    osc.getSamples (origStorage);
    
    const auto& orig = std::as_const (origStorage);
    
    circularBuffer.storeSamples (orig);
    
    beginTest ("Num stored samples stored correctly");
    
    expectEquals (circularBuffer.getNumStoredSamples(), numSamples);
    
    circularBuffer.getSamples (circOutput);
    
    expectEquals (circularBuffer.getNumStoredSamples(), 0);
    
    beginTest ("Store samples and retrieve later");
    
    expect (allSamplesAreEqual (circOutput, orig, 0, numSamples));
    
    beginTest ("Retrieve fewer samples than were passed in");
    
    circularBuffer.storeSamples (orig);
    
    const auto halfNumSamples = numSamples / 2;
    
    auto alias = lemons::dsp::buffers::getAliasBuffer (circOutput, 0, halfNumSamples);
    
    circularBuffer.getSamples (alias);
    
    expect (allSamplesAreEqual (alias, orig, 0, halfNumSamples));
    
    beginTest ("Retrieve more samples than are left in circ buffer");
    
    expectEquals (circularBuffer.getNumStoredSamples(), halfNumSamples);
    
    circularBuffer.getSamples (circOutput);
    
    expect (allSamplesAreZero (circOutput, 0, halfNumSamples));
    
    expect (allSamplesAreEqual (circOutput, orig, halfNumSamples, halfNumSamples));
}

#endif
