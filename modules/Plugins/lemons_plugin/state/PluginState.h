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

	void addTo (juce::AudioProcessor& processor) const;

	void add (Parameter& parameter);

	template <typename... Args>
	void add (Parameter& first, Args&&... rest)
	{
		add (first);
		add (std::forward<Args> (rest)...);
	}

	void processControllerMessage (int number, int value);

	[[nodiscard]] ValueTree saveToValueTree (bool currentProgramOnly) const;

	void loadFromValueTree (const ValueTree& tree);


	Dimensions editorSize { Dimensions::getDefault() };

	ToggleParameter bypass { "Bypass", false };

private:
	juce::Array<Parameter*> params { &bypass };
};

}  // namespace lemons::plugin
