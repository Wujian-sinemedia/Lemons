
/** @ingroup presets
 *  @{
 */
#pragma once

#include <lemons_serializing/lemons_serializing.h>

namespace lemons::plugin
{
struct Preset : SerializableData
{
    String name, category, author, description;

    ValueTree state {name};

    Version version;

private:
    void serialize (TreeReflector& ref) final;
};

}  // namespace lemons::plugin

/** @}*/
