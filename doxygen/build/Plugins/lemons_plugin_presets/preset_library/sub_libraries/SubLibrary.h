
/** @ingroup preset_library
 *  @{
 */

/** @ingroup sub_libraries
 *  @{
 */
#pragma once

#include <lemons_plugin/lemons_plugin.h>

namespace lemons::plugin::presets
{
class SubLibrary
{
public:
    SubLibrary (StateBase&    stateToUse,
                const String& companyNameToUse,
                const String& productNameToUse,
                const String& presetFileExtensionToUse = ".xml");

    virtual ~SubLibrary() = default;

    const juce::Array< Preset >& getPresets();

protected:
    juce::Array< Preset > presets;

    const String companyName;
    const String productName;
    const String presetFileExtension;

    StateBase& state;

private:
    virtual void refreshPresetList() = 0;
};

}  // namespace lemons::plugin::presets

/** @}*/

/** @}*/
