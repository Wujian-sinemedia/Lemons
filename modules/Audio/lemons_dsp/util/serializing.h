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


namespace lemons::serializing
{

template <typename SampleType>
using AudioBuffer = juce::AudioBuffer<SampleType>;

/** @defgroup lemons_audio_buffers Audio buffers
    @ingroup lemons_dsp
    Audio buffer utility functions.
 */


/** @ingroup lemons_audio_buffers lemons_serializing
    Encodes an AudioBuffer as a blob of binary data. Note that the sample rate may be optionally specified. Internally, this uses JUCE's FlacAudioFormat.
    @see audioFromBinary(), saveAudioToFile()
 */
template <typename SampleType>
[[nodiscard]] MemoryBlock audioToBinary (const AudioBuffer<SampleType>& buffer, double samplerate = 48000.);

/** @ingroup lemons_audio_buffers lemons_serializing
    Returns an AudioBuffer from a binary representation of one. Internally, this uses JUCE's FlacAudioFormat.
    @see audioToBinary(), loadAudioFromFile(), binary::Data::getAudio()
 */
template <typename SampleType>
[[nodiscard]] AudioBuffer<SampleType> audioFromBinary (const MemoryBlock& block);

/** @name lemons_audio_channel_set_to_value_tree AudioChannelSet
    @ingroup lemons_dsp lemons_serializing
 */
///@{

[[nodiscard]] juce::AudioChannelSet channelSetFromValueTree (const ValueTree& tree);

[[nodiscard]] ValueTree channelSetToValueTree (const juce::AudioChannelSet& set);

///@}

}  // namespace lemons::serializing


namespace lemons::files
{

template <typename SampleType>
using AudioBuffer = juce::AudioBuffer<SampleType>;

/** @ingroup lemons_audio_buffers lemons_files
    Loads an AudioBuffer from a file. The buffer must have been previously written to the file using the saveAudioBuffer() method. For a more robust, official-format-compatible method of loading audio files, see the AudioFile class.
    @see saveAudioBuffer()
 */
template <typename SampleType>
[[nodiscard]] AudioBuffer<SampleType> loadAudioBuffer (const File& file);


/** @ingroup lemons_audio_buffers lemons_files
    Writes an AudioBuffer to a File in FLAC format. This writes the data in an opaque binary format that can be reloaded by the free functions in this namespace, but may not be compatible with official audio formats.
    @see loadAudioBuffer()
 */
template <typename SampleType>
bool saveAudioBuffer (const AudioBuffer<SampleType>& audio, const File& file, double samplerate = 48000.);

}  // namespace lemons::files

namespace lemons::binary
{

template <typename SampleType>
using AudioBuffer = juce::AudioBuffer<SampleType>;

/** @ingroup lemons_audio_buffers lemons_binary
 */
template <typename SampleType>
[[nodiscard]] AudioBuffer<SampleType> getAudioBuffer (const String& filename);

}  // namespace lemons::binary

/*---------------------------------------------------------------------------------------------------------------------------*/

namespace juce
{

template <>
struct VariantConverter<AudioChannelSet>
{
    static AudioChannelSet fromVar (const var& v);
    static var             toVar (const AudioChannelSet& s);
};

template <>
struct VariantConverter<ADSR::Parameters>
{
	static ADSR::Parameters fromVar (const var& v);
	static var              toVar (const ADSR::Parameters& p);

private:
	static constexpr auto attack  = "Attack";
	static constexpr auto decay   = "Decay";
	static constexpr auto sustain = "Sustain";
	static constexpr auto release = "Release";
};

template <>
struct VariantConverter<AudioBuffer<float>>
{
	static AudioBuffer<float> fromVar (const var& v);
	static var                toVar (const AudioBuffer<float>& b);
};

template <>
struct VariantConverter<AudioBuffer<double>>
{
	static AudioBuffer<double> fromVar (const var& v);
	static var                 toVar (const AudioBuffer<double>& b);
};

}  // namespace juce
