
#pragma once

namespace lemons::serializing
{
String valueTreeToJSON (const ValueTree& v);

ValueTree valueTreeFromJSON (const String& jsonText);

}  // namespace bav::serializing
