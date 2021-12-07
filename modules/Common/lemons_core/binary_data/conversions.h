#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_graphics/juce_graphics.h>


namespace lemons::binary
{

using juce::Image;
using juce::MemoryBlock;
using juce::MidiBuffer;
using juce::ValueTree;

template <typename SampleType>
using AudioBuffer = juce::AudioBuffer<SampleType>;


/** Encodes an Image object as a blob of binary data.
    @see imageFromBinary()
 */
[[nodiscard]] MemoryBlock imageToBinary (const Image& image);

/** Returns an Image object from a binary representation of one.
    @see imageToBinary()
 */
[[nodiscard]] Image imageFromBinary (const MemoryBlock& block);

/** Encodes an AudioBuffer as a blob of binary data. Note that the sample rate may be optionally specified.
    @see audioFromBinary()
 */
template <typename SampleType>
[[nodiscard]] MemoryBlock audioToBinary (const AudioBuffer<SampleType>& buffer, double samplerate = 48000.);

/** Returns an AudioBuffer from a binary representation of one.
    @see audioToBinary()
 */
template <typename SampleType>
[[nodiscard]] AudioBuffer<SampleType> audioFromBinary (const MemoryBlock& block);

/** Encodes a MidiBuffer as a blob of binary data.
    @see midiFromBinary
 */
[[nodiscard]] MemoryBlock midiToBinary (const MidiBuffer& midi);

/** Returns a MidiBufer from a binary representation of one.
    @see midiToBinary
 */
[[nodiscard]] MidiBuffer midiFromBinary (const MemoryBlock& block);

/** Converts a MemoryBlock to a String representation.
    @see memoryBlockFromString()
 */
[[nodiscard]] String memoryBlockToString (const MemoryBlock& block);

/** Returns a MemoryBlock from a String representation of one.
    @see memoryBlockToString()
 */
[[nodiscard]] MemoryBlock memoryBlockFromString (const String& string);

/** Converts an Image to a String representation of one.
    @see imageFromString()
 */
[[nodiscard]] String imageToString (const Image& image);

/** Returns an Image from a String representation of one.
    @see imageToString()
 */
[[nodiscard]] Image imageFromString (const String& string);

/** Converts an AudioBuffer to a String representation of one. Note that the sample rate may be optionally specified.
    @see audioFromString()
 */
template <typename SampleType>
[[nodiscard]] String audioToString (const AudioBuffer<SampleType>& audio, double samplerate = 48000.);

/** Returns an AudioBuffer from a String representation of one.
    @see audioToString()
 */
template <typename SampleType>
[[nodiscard]] AudioBuffer<SampleType> audioFromString (const String& string);

/** Converts a MidiBuffer to a String representation of one.
    @see midiFromString()
 */
[[nodiscard]] String midiToString (const MidiBuffer& midi);

/** Returns a MidiBuffer from a String representation of one.
    @see midiToString()
 */
[[nodiscard]] MidiBuffer midiFromString (const String& string);


/** Converts a ValueTree to a JSON string.
    @see valueTreeFromJSON()
 */
[[nodiscard]] String valueTreeToJSON (const ValueTree& v);

/** Converts a JSON string to a ValueTree.
    @see valueTreeToJSON()
 */
[[nodiscard]] ValueTree valueTreeFromJSON (const String& jsonText);

}  // namespace lemons::binary


/*---------------------------------------------------------------------------------------------------------------------------------*/

#if LEMONS_UNIT_TESTS

#include <lemons_unit_testing/lemons_unit_testing.h>


namespace lemons::tests
{

struct DataConversionTests : public CoreTest
{
	DataConversionTests();

private:
	void runTest() final;

	template <typename SampleType>
	void runTypedTests();
};

static DataConversionTests dataConversionTest;

}  // namespace lemons::tests

#endif
