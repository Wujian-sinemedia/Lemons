#pragma once

#include "StateToggleButtons.h"
#include "PresetSavingComponent/PresetSavingComponent.h"
#include "PresetBrowserComponent/PresetBrowserComponent.h"

namespace lemons::plugin
{
class PresetBar : public juce::Component
{
public:
	PresetBar (  // StateBase&    stateToUse,
	             // StateToggles& toggler,
	    const String& productName,
	    const String& presetFileExtension = ".bvpreset",
	    const String& companyName         = "BenViningMusicSoftware");

private:
	StateToggleButtons toggleButtons;
};

}  // namespace lemons::plugin
