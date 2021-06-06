
#pragma once

namespace bav::serializing
{
extern String valueTreeToJSON (const juce::ValueTree& v);

extern juce::ValueTree valueTreeFromJSON (const String& jsonText);

}  // namespace bav::serializing
