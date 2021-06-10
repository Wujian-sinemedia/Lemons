
#pragma once

namespace bav::serializing
{
extern String valueTreeToJSON (const ValueTree& v);

extern ValueTree valueTreeFromJSON (const String& jsonText);

}  // namespace bav::serializing
