#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

namespace lemons::midi
{

using juce::MidiBuffer;
using juce::MidiMessage;

// copies a range of events from one juce::MidiBuffer to another, optionally providing a sample offset
void copyRangeOfMidiBuffer (const MidiBuffer& readingBuffer,
                            MidiBuffer&       destBuffer,
                            const int         startSampleOfInput,
                            const int         startSampleOfOutput,
                            const int         numSamples);


}  // namespace lemons::midi
