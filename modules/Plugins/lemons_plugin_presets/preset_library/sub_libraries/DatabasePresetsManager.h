#pragma once

#include "SubLibrary.h"
#include "database/UserProfile.h"
#include "database/PresetDatabase.h"

namespace bav::plugin::presets
{
class DatabasePresetsManager : public SubLibrary
{
public:
    DatabasePresetsManager (StateBase&    stateToUse,
                            const String& companyNameToUse,
                            const String& productNameToUse,
                            const String& presetFileExtensionToUse = ".xml");

private:
    void refreshPresetList() final;
};

}  // namespace bav::plugin::presets
