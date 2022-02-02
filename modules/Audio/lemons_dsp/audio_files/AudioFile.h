/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

#pragma once

#include <juce_audio_formats/juce_audio_formats.h>


namespace lemons::dsp
{

using juce::File;
using juce::String;

/** @defgroup audio_files Audio files
	@ingroup lemons_dsp
	Utilities for working with audio files.
 */


/** @ingroup audio_files
	This class represents the data of an audio file, including the AudioBuffer itself, as well as the samplerate, bit depth, and metadata information.
	One of these can be created from an audio file on disk, or from any kind of juce::InputStream.
 */
struct AudioFile final
{
	/** Creates an invalid audio file. */
	AudioFile() = default;

	/** Creates an AudioFile from a file on disk. */
	explicit AudioFile (const File& audioFile);

	/** Creates an AudioFile from any kind of input stream. */
	explicit AudioFile (std::unique_ptr<juce::InputStream> audioStream);

	AudioFile (const AudioFile& other) noexcept;

	AudioFile (AudioFile&& other) noexcept;

	/** Returns the length in seconds of the entire audio file at its original samplerate. */
	[[nodiscard]] double getLengthInSeconds() const noexcept;

	/** Returns true if the file's data was loaded successfully. */
	[[nodiscard]] bool isValid() const noexcept;

	/** Returns true if the file this data represents exists locally on the current device.
		This may return false if the file has been deleted since the construction of this object, or if you used the constructor that takes an input stream.
	*/
	[[nodiscard]] bool existsOnDisk() const noexcept;

	/** Returns the actual audio data of this audio file.
		If isValid() returns false, then this function will return a buffer with a size of 0 channels and 0 samples.
		Internally, the data is always loaded as floats; the first time you call the double version of this function, the data will be converted to double precision internally.
	*/
	template <typename SampleType>
	[[nodiscard]] const AudioBuffer<SampleType>& getData();

	/** Returns the samplerate of the audio file. */
	[[nodiscard]] double getSamplerate() const noexcept;

	/** Returns the number of samples in the audio file. */
	[[nodiscard]] int getNumSamples() const noexcept;

	/** Returns the number of channels in the audio file. */
	[[nodiscard]] int getNumChannels() const noexcept;

	/** Returns the number of bits per sample in this audio file. */
	[[nodiscard]] int getBitsPerSample() const noexcept;

	/** Returns the File object that this audio file was loaded from.
		Note that this may return an invalid File object if you used the constructor that takes an input stream.
	*/
	[[nodiscard]] File getFile() const noexcept;

	/** Returns the name of the audio format, eg, 'wav' or 'aiff'. */
	[[nodiscard]] String getFormatName() const noexcept;

	/** Returns the audio format object for this file's audio format. */
	[[nodiscard]] juce::AudioFormat* getAudioFormat() const;

	/** Returns the audio file's metadata values. */
	[[nodiscard]] const juce::StringPairArray& getMetadata() const noexcept;

	[[nodiscard]] int getReferenceCount() const noexcept;

	bool duplicateIfShared();

private:

	struct AudioFileData;

	using DataPtr = juce::ReferenceCountedObjectPtr<AudioFileData>;

	struct AudioFileData final : public juce::ReferenceCountedObject
	{
		explicit AudioFileData (std::unique_ptr<juce::AudioFormatReader> reader, const File& f);

		[[nodiscard]] bool isValid() const noexcept;

		template <typename SampleType>
		[[nodiscard]] const AudioBuffer<SampleType>& getData();

		[[nodiscard]] double getLengthInSeconds() const noexcept;

		[[nodiscard]] DataPtr clone() const;

		double samplerate { 0. };
		int	   lengthInSamples { 0 }, numChannels { 0 }, bitsPerSample { 0 };

		File file;

		String audioFormat;

		juce::StringPairArray metadata;

	private:

		AudioFileData() = default;

		AudioBuffer<float>	float_data;
		AudioBuffer<double> double_data;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioFileData)
	};

	explicit AudioFile (std::unique_ptr<juce::AudioFormatReader> reader, const File& f);

	DataPtr data;

	JUCE_LEAK_DETECTOR (AudioFile)
};

}  // namespace lemons::dsp

/*---------------------------------------------------------------------------------------------------------------------------*/

namespace lemons::binary
{

/** @ingroup audio_files lemons_binary
	Returns an AudioFile object from a file stored in the BinaryData target. If the audio can't be loaded, an assertion will be thrown.
 */
[[nodiscard]] dsp::AudioFile getAudioFile (const String& audioFileName);

/** @ingroup lemons_binary
	Returns a list of names of binary resource files that have file extensions matching known audio file formats.
 */
[[nodiscard]] juce::StringArray getAudioFileNames();

}  // namespace lemons::binary

/*---------------------------------------------------------------------------------------------------------------------------*/

namespace lemons::files
{

/** Returns true if the file has an extension matching any of the known audio formats.
 */
[[nodiscard]] bool isAudioFile (const File& file);

}  // namespace lemons::files
