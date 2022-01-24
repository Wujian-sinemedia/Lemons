#pragma once

#include <juce_audio_formats/juce_audio_formats.h>


namespace lemons::dsp::formats
{

using juce::MemoryBlock;

/** Returns a static AudioFormatManager used for creating all AudioFiles. */
[[nodiscard]] juce::AudioFormatManager& getDefaultAudioFormatManager();

[[nodiscard]] juce::AudioFormat* getNamedFormat (const String& name);

[[nodiscard]] juce::StringArray getValidFormatNames();

template <typename SampleType>
bool writeAudioToBlock (MemoryBlock& outputBlock,
	const AudioBuffer<SampleType>& buffer, double samplerate,
	juce::AudioFormat& format);

template <typename SampleType>
bool readAudioFromBlock (AudioBuffer<SampleType>& outputBuffer,
	const MemoryBlock& block, juce::AudioFormat& format);


template <typename SampleType>
bool saveAudioToFile (const File&  destFile,
	const AudioBuffer<SampleType>& buffer, double samplerate,
	juce::AudioFormat& format);

template <typename SampleType>
bool loadAudioFromFile (AudioBuffer<SampleType>& outputBuffer,
	const File& sourceFile, juce::AudioFormat& format);

}  // namespace lemons::dsp::formats
