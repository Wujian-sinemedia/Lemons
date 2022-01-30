
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


namespace lemons::serializing
{

using juce::File;
using juce::MemoryBlock;
using juce::MidiBuffer;
using juce::MidiFile;


/** @ingroup lemons_midi lemons_serializing
	Encodes a MidiBuffer as a blob of binary data.
	@see midiBufferFromBinary(), saveMidiToFile()
 */
[[nodiscard]] MemoryBlock midiToBinary (const MidiBuffer& midi);

/** @ingroup lemons_midi lemons_serializing
	Returns a MidiBuffer from a binary representation of one.
	@see midiFileFromBinary(), midiToBinary(), loadMidiFromFile(), binary::Data::getMidi()
 */
[[nodiscard]] MidiBuffer midiBufferFromBinary (const MemoryBlock& block);


/** @ingroup lemons_midi lemons_serializing
	Encodes a MidiFile as a blob of binary data.
	@see midiFileFromBinary(), saveMidiToFile()
 */
[[nodiscard]] MemoryBlock midiToBinary (const MidiFile& midi);


/** @ingroup lemons_midi lemons_serializing
	Returns a MidiFile from a binary representation of one.
	@see midiBufferFromBinary(), midiToBinary(), loadMidiFromFile(), binary::Data::getMidi()
 */
[[nodiscard]] MidiFile midiFileFromBinary (const MemoryBlock& block);


}  // namespace lemons::serializing


namespace lemons::files
{

using juce::MidiBuffer;
using juce::MidiFile;


/** @ingroup lemons_midi lemons_files
	Loads a MidiFile from a file on disk.
	@see saveMidiToFile(), midiBufferFromFile(), binary::Data::getMidi()
 */
[[nodiscard]] MidiFile loadMidiFile (const File& file);

/** @ingroup lemons_midi lemons_files
	Saves a MidiFile to a file on disk.
	@see loadMidiFromFile()
 */
bool saveMidi (const MidiFile& midi, const File& file);


/** @ingroup lemons_midi lemons_files
	Loads a MidiBuffer from a file on disk.
	@see saveMidiToFile(), binary::Data::getMidi()
 */
[[nodiscard]] MidiBuffer loadMidiBuffer (const File& file);

/** @ingroup lemons_midi lemons_files
	Saves a MidiBuffer to a file on disk, formatted as a MidiFile.
	@see loadMidiFromFile(), midiBufferToFile()
 */
bool saveMidi (const MidiBuffer& midi, const File& file);


/** Returns true if the file has an extension matching standard MIDI file extensions.
 */
[[nodiscard]] bool isMidiFile (const File& file);


}  // namespace lemons::files


namespace lemons::binary
{

using juce::String;


/** @ingroup lemons_midi lemons_binary
	Returns a MIDI file object from a MIDI file in the BinaryData target. If the MIDI can't be loaded, an assertion will be thrown.
	@see serializing::loadMidiFromFile(), serializing::saveMidiToFile()
 */
[[nodiscard]] juce::MidiFile getMidiFile (const String& midiFileName);

/** @ingroup lemons_midi lemons_binary
	Returns a MIDI buffer object from a MIDI file in the BinaryData target. If the MIDI can't be loaded, an assertion will be thrown.
	@see serializing::loadMidiFromFile(), serializing::saveMidiToFile()
 */
[[nodiscard]] juce::MidiBuffer getMidiBuffer (const String& midiFileName);


/** Returns a list of names of binary resource files that have file extensions matching standard MIDI file extensions.
 */
[[nodiscard]] juce::StringArray getMidiFileNames();

}  // namespace lemons::binary

/*---------------------------------------------------------------------------------------------------------------------------*/

namespace juce
{

template <>
struct VariantConverter<MidiBuffer>
{
	static MidiBuffer fromVar (const var& v);
	static var		  toVar (const MidiBuffer& b);
};

template <>
struct VariantConverter<MidiFile>
{
	static MidiFile fromVar (const var& v);
	static var		toVar (const MidiFile& f);
};

template <>
struct VariantConverter<MidiMessage>
{
	static MidiMessage fromVar (const var& v);
	static var		   toVar (const MidiMessage& m);

private:

	static constexpr auto data_prop = "Data";
	static constexpr auto time_prop = "Timestamp";
};

}  // namespace juce
