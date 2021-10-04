#pragma once

namespace lemons::serializing
{
/** Explicitly calls juce::var::toString(). */
template <>
String fromVar (juce::var var);

/** Serializes a float Point. */
template <>
juce::var toVar (juce::Point< float >& point);

/** Serializes a float Point. */
template <>
juce::Point< float > fromVar (juce::var var);

/** Serializes an integer Point. */
template <>
juce::var toVar (juce::Point< int >& point);

/** Serializes an integer Point. */
template <>
juce::Point< int > fromVar (juce::var var);

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

/** Serializes a juce::Uuid as a String. */
template <>
juce::var toVar (juce::Uuid& uuid);

/** Serializes a juce::Uuid as a String. */
template <>
juce::Uuid fromVar (juce::var var);

/** Serializes a juce::Time object by saving it as the number of milliseconds since the computing epoch. */
template <>
juce::var toVar (juce::Time& time);

/** Serializes a juce::Time object by loading it as the number of milliseconds since the computing epoch. */
template <>
juce::Time fromVar (juce::var var);

/** Serializes a juce::RelativeTime object. */
template <>
juce::var toVar (juce::RelativeTime& time);

/** Serializes a juce::RelativeTime object. */
template <>
juce::RelativeTime fromVar (juce::var var);

}  // namespace lemons::serializing
