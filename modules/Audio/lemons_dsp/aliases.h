#pragma once

#include <juce_audio_utils/juce_audio_utils.h>

namespace lemons::dsp
{

using juce::String;

using juce::MidiBuffer;
using juce::MidiMessage;

template <typename T>
using AudioBuffer = juce::AudioBuffer<T>;

}  // namespace lemons::dsp
