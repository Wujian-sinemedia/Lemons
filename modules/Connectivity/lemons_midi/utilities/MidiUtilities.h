#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

namespace lemons::midi
{

using juce::MidiBuffer;
using juce::MidiMessage;

template <typename SampleType>
using AudioBuffer = juce::AudioBuffer<SampleType>;

// copies a range of events from one juce::MidiBuffer to another, optionally providing a sample offset
void copyRangeOfMidiBuffer (const MidiBuffer& readingBuffer,
                            MidiBuffer&       destBuffer,
                            const int         startSampleOfInput,
                            const int         startSampleOfOutput,
                            const int         numSamples);


bool midiBuffersAreEqual (const MidiBuffer& buffer1, const MidiBuffer& buffer2);


}  // namespace lemons::midi
