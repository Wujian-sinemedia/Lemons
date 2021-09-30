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

}  // namespace lemons::serializing
