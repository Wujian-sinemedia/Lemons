#pragma once

namespace lemons
{
template <>
String fromVar (juce::var var);

template <>
juce::var toVar (juce::Point< float >& point);

template <>
juce::Point< float > fromVar (juce::var var);

}  // namespace bav
