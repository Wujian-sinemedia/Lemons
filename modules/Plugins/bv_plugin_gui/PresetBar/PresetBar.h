#pragma once

#include <bv_plugin/bv_plugin.h>
#include "StateToggleButtons.h"

namespace bav::plugin
{
class PresetBar : public juce::Component
{
public:
    PresetBar (StateBase&    stateToUse,
               const String& productName,
               const String& presetFileExtension = ".xml",
               const String& companyName         = "BenViningMusicSoftware");

private:
    StateBase& state;

    Undo undoManager {state};

    PresetManager presetManager;

    StateToggleButtons toggleButtons {state};
};

}  // namespace bav::plugin
