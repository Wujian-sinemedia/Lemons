#pragma once

namespace lemons::serializing
{
/** Explicitly calls juce::var::toString(). */
template <>
String fromVar (juce::var var);

/** Serializes a Point. */
template <>
juce::var toVar (juce::Point< float >& point);

/** Serializes a Point. */
template <>
juce::Point< float > fromVar (juce::var var);

/** Serializes a MemoryBlock.
    This works by writing the contents of the memory block to a String use Base64 encoding.
 */
template <>
juce::var toVar (juce::MemoryBlock& block);

/** Serializes a MemoryBlock.
    This works by reading the contents of a String into binary form, using Base64 encoding.
 */
template <>
juce::MemoryBlock fromVar (juce::var var);

/** Serializes a URL as a String. */
template <>
juce::var toVar (juce::URL& url);

/** Serializes a URL as a String. */
template <>
juce::URL fromVar (juce::var var);

/** Serializes a std::string. */
template <>
juce::var toVar (std::string& string);

/** Serializes a std::string. */
template <>
std::string fromVar (juce::var var);

}  // namespace lemons::serializing
