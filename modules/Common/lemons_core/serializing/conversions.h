#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_graphics/juce_graphics.h>


namespace lemons::serializing
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

/** Converts a MIDI buffer to a MIDI file object.
    @see midiBufferFromFile()
 */
[[nodiscard]] juce::MidiFile midiBufferToFile (const MidiBuffer& midi);

/** Converts a MIDI file object to a MIDI buffer.
    @param file The MIDI file object to read from.
    @param trackToRead If this is greater than -1, then only events from the specified track number will be added to the returned buffer. If this is -1, then all events from every track of the MIDI file will be concatenated into the output buffer.
    @see midiBufferToFile()
 */
[[nodiscard]] MidiBuffer midiBufferFromFile (const juce::MidiFile& file, int trackToRead = -1);

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


/** Converts a ValueTree to a JSON string.
    @see valueTreeFromJSON()
 */
[[nodiscard]] String valueTreeToJSON (const ValueTree& v);

/** Converts a JSON string to a ValueTree.
    @see valueTreeToJSON()
 */
[[nodiscard]] ValueTree valueTreeFromJSON (const String& jsonText);

}  // namespace lemons::serializing


/*---------------------------------------------------------------------------------------------------------------------------------*/

#if LEMONS_UNIT_TESTS

#  include <lemons_unit_testing/lemons_unit_testing.h>


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
