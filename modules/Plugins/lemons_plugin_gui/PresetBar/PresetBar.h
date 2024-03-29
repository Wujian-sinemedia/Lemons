/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

#pragma once

#include "PresetBrowserComponent/PresetBrowserComponent.h"
#include "PresetSavingComponent/PresetSavingComponent.h"
#include "StateToggleButtons.h"

namespace lemons::plugin
{
class PresetBar : public juce::Component
{
public:

	explicit PresetBar (  // StateBase&    stateToUse,
						  // StateToggles& toggler,
		const String& productName,
		const String& presetFileExtension = ".bvpreset",
		const String& companyName		  = "BenViningMusicSoftware");

private:

	StateToggleButtons toggleButtons;
};

}  // namespace lemons::plugin
