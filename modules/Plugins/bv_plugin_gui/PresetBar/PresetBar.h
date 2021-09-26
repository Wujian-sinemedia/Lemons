#pragma once

#include <bv_plugin_presets/bv_plugin_presets.h>

#include "StateToggleButtons.h"
#include "PresetSavingComponent/PresetSavingComponent.h"
#include "PresetBrowserComponent/PresetBrowserComponent.h"

namespace bav::plugin
{
class PresetBar : public juce::Component
{
public:
    PresetBar (StateBase&    stateToUse,
               StateToggler& toggler,
               const String& productName,
               const String& presetFileExtension = ".xml",
               const String& companyName         = "BenViningMusicSoftware");

private:
    PresetsLibrary presetLibrary;

    StateToggleButtons toggleButtons;
};

}  // namespace bav::plugin
