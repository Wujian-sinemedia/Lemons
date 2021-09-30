
#pragma once

namespace lemons::serializing
{
/** Serializes an AudioBuffer to a var.
    Internally, this uses juce's FlacAudioFormat to write the buffer's contents into a String.
 */
template <>
juce::var toVar (juce::AudioBuffer< float >& buffer);

/** Serializes an AudioBuffer to a var.
    Since juce's FlacAudioFormat is only available for float samples, this first converts the buffer to floats and then uses juce::FlacAudioFormat to write the buffer's contents to a String.
 */
template <>
juce::var toVar (juce::AudioBuffer< double >& buffer);

/** Deserializes an AudioBuffer from a var.
    Internally, this converts the var to a String, then uses juce's FlacAudioFormat to decode the String back into audio samples.
 */
template <>
juce::AudioBuffer< float > fromVar (juce::var var);

/** Deserializes an AudioBuffer from a var.
    Internally, this converts the var to a String, then uses juce's FlacAudioFormat to decode the String back into audio samples.
    Since juce's FlacAudioFormat is only available for float samples, the buffer is first decoded with float samples and then converted to double samples for the final result.
 */
template <>
juce::AudioBuffer< double > fromVar (juce::var var);

}  // namespace lemons::serializing
