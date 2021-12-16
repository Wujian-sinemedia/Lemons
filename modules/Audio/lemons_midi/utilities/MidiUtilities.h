#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

namespace lemons::midi
{

using juce::String;
using juce::MidiBuffer;
using juce::MidiMessage;

// copies a range of events from one juce::MidiBuffer to another, optionally providing a sample offset
void copyRangeOfMidiBuffer (const MidiBuffer& readingBuffer,
                            MidiBuffer&       destBuffer,
                            const int         startSampleOfInput,
                            const int         startSampleOfOutput,
                            const int         numSamples);

// returns the default name for a specified MIDI controller number
String getControllerName (int controllerNumber);

}  // namespace lemons::midi
