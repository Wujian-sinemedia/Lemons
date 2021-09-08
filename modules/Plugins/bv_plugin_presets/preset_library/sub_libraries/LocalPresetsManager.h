#pragma once

#include "SubLibrary.h"

namespace bav::plugin::presets
{
class LocalPresetsManager : public SubLibrary
/*
 TODO:
 add funcs to load, save, delete, and rename presets
 */
{
public:
    LocalPresetsManager (StateBase&    stateToUse,
                         const String& companyNameToUse,
                         const String& productNameToUse,
                         const String& presetFileExtensionToUse = ".xml");

    virtual ~LocalPresetsManager() = default;

    File presetsFolder();

private:
    void refreshPresetList() final;
};

}  // namespace bav::plugin::presets
