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

namespace lemons::plugin
{

namespace StateVTProperties
{
static constexpr auto fullState    = "PluginState";
static constexpr auto programState = "Program";
static constexpr auto dimensions   = "EditorDimensions";
static constexpr auto customState  = "CustomState";
}  // namespace StateVTProperties

ValueTree State::saveToValueTree (bool currentProgramOnly) const
{
	using namespace StateVTProperties;

	const auto treeType = currentProgramOnly ? programState : fullState;

	ValueTree tree { treeType };

	if (! currentProgramOnly)
	{
		tree.setProperty (dimensions, editorSize.toString(), nullptr);

		tree.appendChild (programs.saveAllToValueTree(), nullptr);
	}

	tree.appendChild (parameters.saveToValueTree(), nullptr);

	const auto customStateData = saveCustomStateData (customState, currentProgramOnly);

	if (customStateData.hasType (customState))
		tree.appendChild (customStateData, nullptr);

	return tree;
}

void State::loadFromValueTree (const ValueTree& tree)
{
	using namespace StateVTProperties;

	if (tree.hasType (fullState))
	{
		if (tree.hasProperty (dimensions))
			editorSize = Dimensions::fromString (tree.getProperty (dimensions).toString());

		programs.restoreAllFromValueTree (tree.getChildWithName (ProgramManager::valueTreeType));
	}

	parameters.loadFromValueTree (tree.getChildWithName (ParameterList::valueTreeType));

	const auto customStateData = tree.getChildWithName (customState);

	if (customStateData.isValid())
		loadCustomStateData (customStateData);
}

ValueTree State::saveCustomStateData (const String&, bool) const
{
	return {};
}

void State::loadCustomStateData (const ValueTree&)
{
}

}  // namespace lemons::plugin
