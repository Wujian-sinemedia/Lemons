#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

namespace lemons::serializing
{

using juce::File;
using juce::MemoryBlock;
using juce::MidiBuffer;
using juce::MidiFile;

/** Converts a MIDI buffer to a MIDI file object.
    @see midiBufferFromFile()
 */
[[nodiscard]] MidiFile midiBufferToFile (const MidiBuffer& midi);

/** Converts a MIDI file object to a MIDI buffer.
    @param file The MIDI file object to read from.
    @param trackToRead If this is greater than -1, then only events from the specified track number will be added to the returned buffer. If this is -1, then all events from every track of the MIDI file will be concatenated into the output buffer.
    @see midiBufferToFile()
 */
[[nodiscard]] MidiBuffer midiBufferFromFile (const MidiFile& file, int trackToRead = -1);

/** Encodes a MidiBuffer as a blob of binary data.
    @see midiBufferFromBinary(), saveMidiToFile()
 */
[[nodiscard]] MemoryBlock midiToBinary (const MidiBuffer& midi);

/** Encodes a MidiFile as a blob of binary data.
    @see midiFileFromBinary(), saveMidiToFile()
 */
[[nodiscard]] MemoryBlock midiToBinary (const MidiFile& midi);

/** Returns a MidiBufer from a binary representation of one.
    @see midiFileFromBinary(), midiToBinary(), loadMidiFromFile(), binary::Data::getMidi()
 */
[[nodiscard]] MidiBuffer midiBufferFromBinary (const MemoryBlock& block);

/** Returns a MidiFile from a binary representation of one.
    @see midiBufferFromBinary(), midiToBinary(), loadMidiFromFile(), binary::Data::getMidi()
 */
[[nodiscard]] MidiFile midiFileFromBinary (const MemoryBlock& block);

/** Saves a MidiFile to a file on disk.
    @see loadMidiFromFile()
 */
bool saveMidiToFile (const MidiFile& midi, const File& file);

/** Saves a MidiBuffer to a file on disk, formatted as a MidiFile.
    @see loadMidiFromFile(), midiBufferToFile()
 */
bool saveMidiToFile (const MidiBuffer& midi, const File& file);

/** Loads a MidiFile from a file on disk.
    @see saveMidiToFile(), midiBufferFromFile(), binary::Data::getMidi()
 */
[[nodiscard]] MidiFile loadMidiFromFile (const File& file);

/** Loads a MidiBuffer from a file on disk.
    @see saveMidiToFile(), binary::Data::getMidi()
 */
[[nodiscard]] MidiBuffer loadMidiBufferFromFile (const File& file);

}  // namespace lemons::serializing


namespace lemons::binary
{

using juce::String;

/** Returns a MIDI file object from a MIDI file in the BinaryData target. If the MIDI can't be loaded, an assertion will be thrown.
    @see serializing::loadMidiFromFile(), serializing::saveMidiToFile()
 */
[[nodiscard]] juce::MidiFile getMidiFile (const String& midiFileName);

/** Returns a MIDI buffer object from a MIDI file in the BinaryData target. If the MIDI can't be loaded, an assertion will be thrown.
    @see serializing::loadMidiFromFile(), serializing::saveMidiToFile()
 */
[[nodiscard]] juce::MidiBuffer getMidiBuffer (const String& midiFileName);

}

/*---------------------------------------------------------------------------------------------------------------------------*/

namespace juce
{

template <>
struct VariantConverter<MidiBuffer>
{
	static MidiBuffer fromVar (const var& v);
	static var        toVar (const MidiBuffer& b);
};

template <>
struct VariantConverter<MidiFile>
{
	static MidiFile fromVar (const var& v);
	static var      toVar (const MidiFile& f);
};

template <>
struct VariantConverter<MidiMessage>
{
	static MidiMessage fromVar (const var& v);
	static var         toVar (const MidiMessage& m);

private:
	static constexpr auto data_prop = "Data";
	static constexpr auto time_prop = "Timestamp";
};

}  // namespace juce

/*---------------------------------------------------------------------------------------------------------------------------*/

namespace lemons::tests
{

struct MidiSerializingTests : public CoreTest
{
	MidiSerializingTests();

private:
	void runTest() final;
};

static MidiSerializingTests midiSerializingTest;

}  // namespace lemons::tests
