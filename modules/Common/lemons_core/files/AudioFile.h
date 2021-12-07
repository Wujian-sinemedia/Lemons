#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_formats/juce_audio_formats.h>

namespace lemons
{
using juce::File;


struct AudioFile final
{
	explicit AudioFile (const File& audioFile);
	explicit AudioFile (std::unique_ptr<juce::InputStream> audioStream);

	[[nodiscard]] double getLengthInSeconds() const noexcept;

	[[nodiscard]] bool isValid() const noexcept;

	[[nodiscard]] bool existsOnDisk() const noexcept;

	double samplerate { 0. };
	int    lengthInSamples { 0 };
	int    numChannels { 0 };
	int    bitsPerSample { 0 };

	juce::StringPairArray metadata;

	File file;

	String audioFormat;

	juce::AudioBuffer<float> data;

private:
	AudioFile (juce::AudioFormatReader* reader, const File& f);
};

}  // namespace lemons
