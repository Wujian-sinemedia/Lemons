
#pragma once

namespace lemons::serializing
{
/** Converts a ValueTree to a JSON string. */
[[nodiscard]] String valueTreeToJSON (const ValueTree& v);


/** Converts a JSON string to a ValueTree. */
[[nodiscard]] ValueTree valueTreeFromJSON (const String& jsonText);

}  // namespace lemons::serializing
