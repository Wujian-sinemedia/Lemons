#pragma once

namespace lemons::dsp::buffers
{
/** Copies all channels from the source buffer to the destination buffer. */
template <typename SampleType>
void copy (const AudioBuffer<SampleType>& source, AudioBuffer<SampleType>& dest);

/** Converts all channels from the source buffer to the other numeric type for the destination buffer. */
template <typename Type1, typename Type2>
void convert (const AudioBuffer<Type1>& source, AudioBuffer<Type2>& dest);

/** Returns an alias buffer referring to the memory region owned by the original passed buffer.
    If the numChannels argument is -1, the alias buffer will have the same number of channels as the buffer being aliased.
 */
template <typename SampleType>
[[nodiscard]] AudioBuffer<SampleType> getAliasBuffer (AudioBuffer<SampleType>& bufferToAlias,
                                                      int                      startSample,
                                                      int                      numSamples,
                                                      int                      numChannels   = -1,
                                                      int                      channelOffset = 0);

/** Returns true if every sample in the given range of the two buffers is equal.
    If startIndex2 is -1, then it will be set to be the same as startIndex1. Similarly, if channel2 is -1, then it will be set to be the same as channel1.
 */
template <typename FloatType>
[[nodiscard]] bool allSamplesAreEqual (const AudioBuffer<FloatType>& buffer1,
                                       const AudioBuffer<FloatType>& buffer2,
                                       int startIndex1, int numSamples,
                                       int startIndex2 = -1,
                                       int channel1    = 0,
                                       int channel2    = -1);

/** Returns true if every sample of every channel of both buffers are equal.
 */
template <typename FloatType>
[[nodiscard]] bool buffersAreEqual (const AudioBuffer<FloatType>& buffer1,
                                    const AudioBuffer<FloatType>& buffer2);

/** Returns true if every sample in the given range of the buffer is exactly zero.
 */
template <typename FloatType>
[[nodiscard]] bool allSamplesAreZero (const AudioBuffer<FloatType>& buffer,
                                      int startIndex, int numSamples, int channel = 0);

}  // namespace lemons::dsp::buffers


/*---------------------------------------------------------------------------------------------------------------------------------*/


#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

template <typename FloatType>
struct AudioBufferUtilsTests : public juce::UnitTest
{
public:
	AudioBufferUtilsTests();

private:
	void runTest() final;

	void resizeAllBuffers (int numSamples, int numChannels);

	dsp::osc::Sine<FloatType> osc;

	juce::AudioBuffer<FloatType> bufferA, bufferB;
};

static AudioBufferUtilsTests<float>  audioBufferUtilsTest_float;
static AudioBufferUtilsTests<double> audioBufferUtilsTest_double;

}  // namespace lemons::tests

#endif
