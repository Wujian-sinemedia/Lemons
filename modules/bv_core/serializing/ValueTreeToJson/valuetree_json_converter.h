
#pragma once

namespace bav::serializing
{
extern juce::String valueTreeToJSON (const juce::ValueTree& v);

extern juce::ValueTree valueTreeFromJSON (const juce::String& jsonText);

}  // namespace bav::serializing
