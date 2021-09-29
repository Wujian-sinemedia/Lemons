#pragma once

#include "sub_libraries/LocalPresetsManager.h"
#include "sub_libraries/FactoryPresetsManager.h"
#include "sub_libraries/DatabasePresetsManager.h"

namespace lemons::plugin
{
class PresetsLibrary
{
public:
    PresetsLibrary (StateBase&    stateToUse,
                    const String& companyNameToUse,
                    const String& productNameToUse,
                    const String& presetFileExtensionToUse = ".xml");

    const juce::Array< Preset >& getPresets();

private:
    void rescanPresets();

    presets::DatabasePresetsManager databasePresets;
    presets::FactoryPresetsManager  factoryPresets;
    presets::LocalPresetsManager    localPresets;

    std::vector< presets::SubLibrary* > sub_libraries = {&databasePresets, &factoryPresets, &localPresets};

    juce::Array< Preset > presets;
};

}  // namespace lemons::plugin
