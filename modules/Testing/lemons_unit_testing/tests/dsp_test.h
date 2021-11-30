#pragma once

#include <juce_audio_basics/juce_audio_basics.h>


namespace lemons::tests
{

struct DspTest : public Test
{
	DspTest (const String& testName);

protected:
    DspTest (const String& testName, const String& testCategory);
    
	template <typename FloatType>
	using AudioBuffer = juce::AudioBuffer<FloatType>;

	using MidiBuffer = juce::MidiBuffer;
    
    template<typename SampleType>
    [[nodiscard]] static String getDspTestName (const String& name);

    [[nodiscard]] static const std::vector<int>&    getTestingBlockSizes();
    [[nodiscard]] static const std::vector<double>& getTestingSamplerates();

	template <typename FloatType>
	[[nodiscard]] static bool buffersAreEqual (const AudioBuffer<FloatType>& buffer1,
	                                           const AudioBuffer<FloatType>& buffer2);

    template<typename FloatType>
    [[nodiscard]] static bool buffersAreReasonablyEqual (const AudioBuffer<FloatType>& buffer1,
                                                         const AudioBuffer<FloatType>& buffer2);
    
	template <typename FloatType>
	[[nodiscard]] static bool allSamplesAreEqual (const AudioBuffer<FloatType>& buffer1,
	                                              const AudioBuffer<FloatType>& buffer2,
	                                              int startIndex1, int numSamples,
	                                              int startIndex2 = -1,
	                                              int channel1    = 0,
	                                              int channel2    = -1);

    
	template <typename FloatType>
	[[nodiscard]] static bool allSamplesAreZero (const AudioBuffer<FloatType>& buffer,
	                                             int startIndex, int numSamples, int channel = 0);
    
    template <typename FloatType>
    [[nodiscard]] static bool bufferIsSilent (const AudioBuffer<FloatType>& buffer);

	[[nodiscard]] static bool midiBuffersAreEqual (const MidiBuffer& buffer1, const MidiBuffer& buffer2);
    
    void fillMidiBufferWithRandomEvents (MidiBuffer& buffer, int numEvents);
    
    template<typename SampleType>
    void fillAudioBufferWithRandomNoise (AudioBuffer<SampleType>& buffer);
};

}  // namespace lemons::tests
