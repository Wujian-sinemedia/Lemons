/*
 ======================================================================================

 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.

 ======================================================================================
 */

#pragma once

#include <lemons_core/lemons_core.h>

namespace lemons::plugin
{

/** Base class for a plugin's entire state.
 */
class State
{
public:

	[[nodiscard]] ToggleParameter& getBypass() { return bypass; }
    
    [[nodiscard]] ValueTree saveToValueTree() const { return {}; }
    
    void loadFromValueTree (const ValueTree& tree) { };

    
	Dimensions editorSize { Dimensions::getDefault() };

	ToggleParameter bypass { "Bypass", false };
};

}  // namespace lemons::plugin
