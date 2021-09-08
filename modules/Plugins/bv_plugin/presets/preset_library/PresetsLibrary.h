#pragma once

#include "LocalPresetsManager.h"
#include "FactoryPresetsManager.h"
#include "DatabasePresetsManager.h"

namespace bav::plugin
{
class PresetsLibrary
{
public:
    PresetsLibrary (StateBase&    stateToUse,
                    const String& companyNameToUse,
                    const String& productNameToUse,
                    const String& presetFileExtensionToUse = ".xml",
                    UndoManager*  um                       = nullptr);

    const juce::Array< Preset >& getPresets();

private:
    void rescanPresets();

    DatabasePresetsManager databasePresets;
    FactoryPresetsManager  factoryPresets;
    LocalPresetsManager    localPresets;

    juce::Array< Preset > presets;
};

}  // namespace bav::plugin
