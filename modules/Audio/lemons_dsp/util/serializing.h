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


/** Encodes an AudioBuffer as a blob of binary data. Note that the sample rate may be optionally specified. Internally, this uses JUCE's FlacAudioFormat.
    @see audioFromBinary(), saveAudioToFile()
 */
template <typename SampleType>
[[nodiscard]] MemoryBlock audioToBinary (const AudioBuffer<SampleType>& buffer, double samplerate = 48000.);

/** Returns an AudioBuffer from a binary representation of one. Internally, this uses JUCE's FlacAudioFormat.
    @see audioToBinary(), loadAudioFromFile(), binary::Data::getAudio()
 */
template <typename SampleType>
[[nodiscard]] AudioBuffer<SampleType> audioFromBinary (const MemoryBlock& block);

/** Writes an AudioBuffer to a File in FLAC format. This writes the data in an opaque binary format that can be reloaded by the free functions in this namespace, but may not be compatible with official audio formats.
    @see loadAudioFromFile(), audioToBinary()
 */
template <typename SampleType>
bool saveAudioToFile (const AudioBuffer<SampleType>& buffer, const File& file, double samplerate = 48000.);

/** Loads an AudioBuffer from a file. The buffer must have been previously written to the file using the saveAudioToFile() method. For a more robust, official-format-compatible method of loading audio files, see the AudioFile class.
    @see saveAudioToFile(), audioFromBinary(), AudioFile, binary::Data::getAudio()
 */
template <typename SampleType>
[[nodiscard]] AudioBuffer<SampleType> loadAudioFromFile (const File& file);

}  // namespace lemons::serializing

/*---------------------------------------------------------------------------------------------------------------------------*/

namespace juce
{

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

//template <>
//struct VariantConverter<PluginDescription>
//{
//	static PluginDescription fromVar (const var& v);
//	static var               toVar (const PluginDescription& d);
//};

}  // namespace juce

/*---------------------------------------------------------------------------------------------------------------------------*/

namespace lemons::tests
{

struct DspSerializingTests : public CoreTest
{
	DspSerializingTests();

private:
	void runTest() final;

	template <typename SampleType>
	void runAudioBufferTest();
};

static DspSerializingTests dspSerializingTest;

}  // namespace lemons::tests
