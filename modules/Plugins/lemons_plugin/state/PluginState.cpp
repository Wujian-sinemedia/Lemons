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

State::State()
{
    add (bypass);
}

void State::addTo (juce::AudioProcessor& processor) const
{
    std::for_each (params.begin(), params.cend(), [&](Parameter* p){ processor.addParameter (p); });
}

void State::add (Parameter& parameter)
{
	params.emplace_back (&parameter);
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

void State::resetAllControllerMappedParams()
{
    for (auto* param : params)
        if (param->isMidiControllerMapped())
            param->resetToDefault();
}

Parameter* State::getNamedParameter (const String& name) const
{
    for (auto* param : params)
        if (param->getParameterName() == name)
            return param;
    
    return nullptr;
}


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

	for (const auto* param : params)
		tree.appendChild (param->saveToValueTree(), nullptr);
    
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

	for (auto* param : params)
	{
        param->loadFromValueTree (tree.getChildWithProperty (Parameter::id_prop, param->getParameterID()));
        param->refreshDefault();
	}
    
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

/*-----------------------------------------------------------------------------------------------------------------*/

State::Listener::Listener (const State& state,
                           std::function<void (Parameter&)>
                               onParamChange,
                           std::function<void (Parameter&, bool)>
                               onGestureGhange)
{
	for (auto* param : state.params)
	{
		const auto change = [=]
		{
			if (onParamChange)
				onParamChange (*param);
		};

		const auto gesture = [=] (bool starting)
		{
			if (onGestureGhange)
				onGestureGhange (*param, starting);
		};

		updaters.add (new ParamUpdater (*param, std::move (change), std::move (gesture)));
	}
}

}  // namespace lemons::plugin
