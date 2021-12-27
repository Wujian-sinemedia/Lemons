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

void State::addTo (juce::AudioProcessor& processor) const
{
	for (auto* param : params)
		processor.addParameter (param);
}

void State::add (Parameter& parameter)
{
	jassert (! params.contains (&parameter));

	params.add (&parameter);
}

void State::processControllerMessage (int number, int value)
{
    for (auto* param : params)
        param->processNewControllerMessage (number, value);
}

bool State::isControllerMapped (int number) const
{
    for (const auto* param : params)
        if (param->getMidiControllerNumber() == number)
            return true;
    
    return false;
}


namespace StateVTProperties
{
static constexpr auto fullState    = "PluginState";
static constexpr auto programState = "Program";
static constexpr auto dimensions   = "EditorDimensions";
}  // namespace StateVTProperties

ValueTree State::saveToValueTree (bool currentProgramOnly) const
{
	using namespace StateVTProperties;

	const auto treeType = currentProgramOnly ? programState : fullState;

	ValueTree tree { treeType };

	if (! currentProgramOnly)
	{
		tree.setProperty (dimensions, editorSize.toString(), nullptr);

		// save all other programs
	}

	for (const auto* param : params)
		tree.appendChild (param->saveToValueTree(), nullptr);

	return tree;
}

void State::loadFromValueTree (const ValueTree& tree)
{
	using namespace StateVTProperties;

	if (tree.hasType (fullState))
	{
		if (tree.hasProperty (dimensions))
			editorSize = Dimensions::fromString (tree.getProperty (dimensions).toString());

		// load all other programs
	}

	for (auto* param : params)
		param->loadFromValueTree (tree.getChildWithProperty (Parameter::id_prop, param->getParameterID()));
}

}  // namespace lemons::plugin
