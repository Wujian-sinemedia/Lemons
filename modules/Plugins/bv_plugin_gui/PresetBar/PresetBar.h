#pragma once

#include <bv_plugin/bv_plugin.h>
#include "StateToggleButtons.h"

namespace bav::plugin
{
class PresetBar : public juce::Component
{
public:
    PresetBar (State&        stateToUse,
               const String& productName,
               const String& presetFileExtension = ".xml",
               const String& companyName         = "BenViningMusicSoftware");

private:
    State& state;

    Undo undoManager {state.getParameters()};

    PresetManager presetManager;

    StateToggleButtons toggleButtons {state};
};

}  // namespace bav::plugin
