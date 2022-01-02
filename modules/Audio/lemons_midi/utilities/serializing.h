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

using juce::File;
using juce::MemoryBlock;
using juce::MidiBuffer;
using juce::MidiFile;


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

}  // namespace lemons::serializing


namespace lemons::files
{

using juce::MidiBuffer;
using juce::MidiFile;

/** Loads a MidiFile from a file on disk.
    @see saveMidiToFile(), midiBufferFromFile(), binary::Data::getMidi()
 */
[[nodiscard]] MidiFile loadMidiFile (const File& file);

/** Loads a MidiBuffer from a file on disk.
    @see saveMidiToFile(), binary::Data::getMidi()
 */
[[nodiscard]] MidiBuffer loadMidiBuffer (const File& file);

/** Saves a MidiFile to a file on disk.
    @see loadMidiFromFile()
 */
bool saveMidi (const MidiFile& midi, const File& file);

/** Saves a MidiBuffer to a file on disk, formatted as a MidiFile.
    @see loadMidiFromFile(), midiBufferToFile()
 */
bool saveMidi (const MidiBuffer& midi, const File& file);

}  // namespace lemons::files


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

}  // namespace lemons::binary

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

#if LEMONS_UNIT_TESTS

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

#endif
