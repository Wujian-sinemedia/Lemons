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


[[nodiscard]] Image imageFromBinary (const MemoryBlock& block);

[[nodiscard]] MemoryBlock imageToBinary (const Image& image);

template <typename SampleType>
[[nodiscard]] AudioBuffer<SampleType> audioFromBinary (const MemoryBlock& block);

template <typename SampleType>
[[nodiscard]] MemoryBlock audioToBinary (const AudioBuffer<SampleType>& buffer);

[[nodiscard]] MidiBuffer midiFromBinary (const MemoryBlock& block);

[[nodiscard]] MemoryBlock midiToBinary (const MidiBuffer& midi);

[[nodiscard]] String memoryBlockToString (const MemoryBlock& block);

[[nodiscard]] MemoryBlock memoryBlockFromString (const String& string);

/** Converts a ValueTree to a JSON string. */
[[nodiscard]] String valueTreeToJSON (const ValueTree& v);

/** Converts a JSON string to a ValueTree. */
[[nodiscard]] ValueTree valueTreeFromJSON (const String& jsonText);

}  // namespace lemons::binary


/*---------------------------------------------------------------------------------------------------------------------------------*/

#if LEMONS_UNIT_TESTING

namespace lemons::tests
{



}

#endif
