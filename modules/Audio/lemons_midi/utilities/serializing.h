#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

namespace lemons::serializing
{

using juce::MidiBuffer;
using juce::MemoryBlock;

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

}
