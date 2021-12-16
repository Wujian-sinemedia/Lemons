/*
 ======================================================================================
 
 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 
 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 
 ======================================================================================
 */

#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_formats/juce_audio_formats.h>


namespace lemons
{

using juce::String;
using juce::File;

template <typename T>
using AudioBuffer = juce::AudioBuffer<T>;


/** This class represents the data of an audio file, including the AudioBuffer itself, as well as the samplerate, bit depth, and metadata information.
 */
struct AudioFile final
{
	/** Creates an AudioFile from a file on disk. */
	explicit AudioFile (const File& audioFile);

	/** Creates an AudioFile from any kind of input stream. */
	explicit AudioFile (std::unique_ptr<juce::InputStream> audioStream);

	/** Destructor. */
	~AudioFile() = default;

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

	/** Returns the audio file's metadata values. */
	[[nodiscard]] const juce::StringPairArray& getMetadata() const noexcept;

	/** Returns a static AudioFormatManager used for creating all AudioFiles. */
	[[nodiscard]] static juce::AudioFormatManager& getFormatManager();
    
    /** Returns true if the file has a file extension matching any of the audio file extensions that the default AudioFormatManager can handle. */
    [[nodiscard]] static bool isValidAudioFile (const File& file);

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
