#pragma once

#include <juce_audio_basics/juce_audio_basics.h>


namespace lemons::tests
{

struct DspTest : public Test
{
	DspTest (const String& testName);

protected:
	template <typename FloatType>
	using AudioBuffer = juce::AudioBuffer<FloatType>;

	using MidiBuffer = juce::MidiBuffer;

    static const std::vector<int>&    getTestingBlockSizes();
	static const std::vector<double>& getTestingSamplerates();

	void logBlocksizeMessage (int blocksize);
    void logSamplerateMessage (double samplerate);

	/** Returns true if every sample of every channel of both buffers are equal.
	 */
	template <typename FloatType>
	[[nodiscard]] static bool buffersAreEqual (const AudioBuffer<FloatType>& buffer1,
	                                           const AudioBuffer<FloatType>& buffer2);

	/** Returns true if every sample in the given range of the two buffers is equal.
	 If startIndex2 is -1, then it will be set to be the same as startIndex1. Similarly, if channel2 is -1, then it will be set to be the same as channel1.
	 */
	template <typename FloatType>
	[[nodiscard]] static bool allSamplesAreEqual (const AudioBuffer<FloatType>& buffer1,
	                                              const AudioBuffer<FloatType>& buffer2,
	                                              int startIndex1, int numSamples,
	                                              int startIndex2 = -1,
	                                              int channel1    = 0,
	                                              int channel2    = -1);

	/** Returns true if every sample in the given range of the buffer is exactly zero.
	 */
	template <typename FloatType>
	[[nodiscard]] static bool allSamplesAreZero (const AudioBuffer<FloatType>& buffer,
	                                             int startIndex, int numSamples, int channel = 0);
    
    template <typename FloatType>
    [[nodiscard]] static bool bufferIsSilent (const AudioBuffer<FloatType>& buffer);

	[[nodiscard]] static bool midiBuffersAreEqual (const MidiBuffer& buffer1, const MidiBuffer& buffer2);
};

}  // namespace lemons::tests
