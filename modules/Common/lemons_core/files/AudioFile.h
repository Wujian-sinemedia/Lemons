#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_formats/juce_audio_formats.h>

namespace lemons
{
using juce::File;

template <typename T>
using AudioBuffer = juce::AudioBuffer<T>;


struct AudioFile final
{
	explicit AudioFile (const File& audioFile);
	explicit AudioFile (std::unique_ptr<juce::InputStream> audioStream);

	~AudioFile() = default;

	[[nodiscard]] double getLengthInSeconds() const noexcept;

	[[nodiscard]] bool isValid() const noexcept;

	[[nodiscard]] bool existsOnDisk() const noexcept;

	template <typename SampleType>
	[[nodiscard]] const AudioBuffer<SampleType>& getData();

	[[nodiscard]] double getSamplerate() const noexcept;

	[[nodiscard]] int getNumSamples() const noexcept;

	[[nodiscard]] int getNumChannels() const noexcept;

	[[nodiscard]] int getBitsPerSample() const noexcept;

	[[nodiscard]] File getFile() const noexcept;

	[[nodiscard]] String getFormatName() const noexcept;

	[[nodiscard]] const juce::StringPairArray& getMetadata() const noexcept;

private:
	explicit AudioFile (juce::AudioFormatReader* reader, const File& f);

	AudioBuffer<float>  float_data;
	AudioBuffer<double> double_data;

	double samplerate { 0. };
	int    lengthInSamples { 0 };
	int    numChannels { 0 };
	int    bitsPerSample { 0 };

	File file;

	String audioFormat;

	juce::StringPairArray metadata;
};

}  // namespace lemons
