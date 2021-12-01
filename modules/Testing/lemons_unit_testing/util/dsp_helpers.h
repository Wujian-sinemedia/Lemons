#pragma once

#include <juce_audio_basics/juce_audio_basics.h>


namespace lemons::tests
{

template <typename FloatType>
using AudioBuffer = juce::AudioBuffer<FloatType>;

using MidiBuffer = juce::MidiBuffer;

template <typename FloatType>
[[nodiscard]] bool buffersAreEqual (const AudioBuffer<FloatType>& buffer1,
                                    const AudioBuffer<FloatType>& buffer2);

template <typename FloatType>
[[nodiscard]] bool buffersAreReasonablyEqual (const AudioBuffer<FloatType>& buffer1,
                                              const AudioBuffer<FloatType>& buffer2);

template <typename FloatType>
[[nodiscard]] bool allSamplesAreEqual (const AudioBuffer<FloatType>& buffer1,
                                       const AudioBuffer<FloatType>& buffer2,
                                       int startIndex1, int numSamples,
                                       int startIndex2 = -1,
                                       int channel1    = 0,
                                       int channel2    = -1);


template <typename FloatType>
[[nodiscard]] bool allSamplesAreZero (const AudioBuffer<FloatType>& buffer,
                                      int startIndex, int numSamples, int channel = 0);

template <typename FloatType>
[[nodiscard]] bool bufferIsSilent (const AudioBuffer<FloatType>& buffer);

template <typename FloatType>
[[nodiscard]] bool allSamplesAreValid (const AudioBuffer<FloatType>& buffer);

[[nodiscard]] bool midiBuffersAreEqual (const MidiBuffer& buffer1, const MidiBuffer& buffer2);

}  // namespace lemons::tests
