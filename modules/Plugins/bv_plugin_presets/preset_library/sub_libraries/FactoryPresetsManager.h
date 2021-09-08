#pragma once

#include "SubLibrary.h"

namespace bav::plugin::presets
{
class FactoryPresetsManager : public SubLibrary
/*
 - should write the factory presets to the filesystem if they're missing
 */
{
public:
    FactoryPresetsManager (StateBase&    stateToUse,
                           const String& companyNameToUse,
                           const String& productNameToUse,
                           const String& presetFileExtensionToUse = ".xml");

private:
    void refreshPresetList() final;
};

}  // namespace bav::plugin::presets
