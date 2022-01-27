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


namespace lemons::plugin
{

/** @ingroup plugin_state
	Base class for a plugin's entire state.
 */
class State
{
public:

	State() = default;

	explicit State (const ParameterLayout& parameterLayout);

	virtual ~State() = default;

	[[nodiscard]] ValueTree saveToValueTree (bool currentProgramOnly) const;

	void loadFromValueTree (const ValueTree& tree);


	Dimensions editorSize { Dimensions::getDefault() };

	ParameterList parameters;

	ProgramManager programs;

private:

	[[nodiscard]] virtual ValueTree saveCustomStateData (const String& valueTreeType, bool currentProgramOnly) const;

	virtual void loadCustomStateData (const ValueTree& tree);
};

}  // namespace lemons::plugin
