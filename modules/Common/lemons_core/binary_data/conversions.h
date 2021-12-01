#pragma once

#include <juce_audio_basics/juce_audio_basics.h>


namespace lemons::binary
{

using juce::MemoryBlock;
using juce::Image;
using juce::MidiBuffer;

template<typename SampleType>
using AudioBuffer = juce::AudioBuffer<SampleType>;


Image imageFromBinary (const MemoryBlock& block);

MemoryBlock imageToBinary (const Image& image);

template<typename SampleType>
AudioBuffer<SampleType> audioFromBinary (const MemoryBlock& block);

template<typename SampleType>
MemoryBlock audioToBinary (const AudioBuffer<SampleType>& buffer);

MidiBuffer midiFromBinary (const MemoryBlock& block);

MemoryBlock midiToBinary (const MidiBuffer& midi);

String memoryBlockToString (const MemoryBlock& block);

MemoryBlock memoryBlockFromString (const String& string);

}
