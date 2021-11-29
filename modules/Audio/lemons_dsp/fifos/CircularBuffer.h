#pragma once

namespace lemons::dsp
{

/**
    A circular buffer meant for storing a single channel's worth of audio samples.
    If you need to store more than one channel's worth of samples, use the AudioFifo class.
    @see AudioFifo
 */
template <typename SampleType>
class CircularBuffer final
{
public:
    /** Creates a CircularBuffer with an initial size. */
    explicit CircularBuffer (int initialCapacity = 512);

    /** Stores samples in the buffer.
        If the buffer's capacity isn't big enough to hold all the passed samples, an assertion will be thrown.
     */
	void storeSamples (const AudioBuffer<SampleType>& samples, int channel = 0);
    
    /** Stores samples in the buffer.
        If the buffer's capacity isn't big enough to hold all the passed samples, an assertion will be thrown.
     */
	void storeSamples (const SampleType* samples, int numSamples);

    /** Reads samples from the buffer.
        If you request more samples than are in the buffer, the first section of the output buffer will be filled with zeroes.
     */
	void getSamples (AudioBuffer<SampleType>& output, int channel = 0);
    
    /** Reads samples from the buffer.
        If you request more samples than are in the buffer, the first section of the output buffer will be filled with zeroes.
     */
	void getSamples (SampleType* output, int numSamples);

    /** Returns the total capacity of the buffer. */
	int getCapacity() const noexcept;
    
    /** Returns the number of samples currently stored in the buffer. */
	int getNumStoredSamples() const noexcept;

    /** Changes the total capacity of the buffer. Calling this method also clears the buffer.
        @see clear()
     */
	void resize (int newSize);

    /** Clears any previously stored data from the buffer. */
	void clear();

private:
	AudioBuffer<SampleType> buffer;
    
    juce::AbstractFifo fifo;
};

}  // namespace lemons::dsp


/*---------------------------------------------------------------------------------------------------------------------------------*/


#if LEMONS_UNIT_TESTS

struct CircularBufferTests : public juce::UnitTest
{
public:
    CircularBufferTests();
    
private:
    using FloatType = float;
    
    void initialise() final;
    
    void runTest() final;
    
    void resizeAllBuffers (int newSize);
    
    lemons::dsp::osc::Sine<FloatType> osc;
    
    juce::AudioBuffer<FloatType> origStorage, circOutput;
    
    lemons::dsp::CircularBuffer<FloatType> circularBuffer;
};

static CircularBufferTests circularBufferTest;

#endif
