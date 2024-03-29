
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

namespace lemons::PluginMetadataEditor::gui
{

BusesLayoutComponent::BusesLayoutComponent (plugin::ProcessorAttributes& attributes, std::function<void()> changedCallback)
	: processorAttributes (attributes), onChange (changedCallback)
{
	jassert (onChange != nullptr);
}

ValueTree BusesLayoutComponent::getCurrentState() const
{
	return busesPropertiesToValueTree (currentState);
}

void BusesLayoutComponent::setState (const ValueTree& tree)
{
	currentState = busesPropertiesFromValueTree (tree);
}

void BusesLayoutComponent::stateChanged()
{
	processorAttributes.defaultBusesLayout = getCurrentState();
	onChange();
}

}  // namespace lemons::PluginMetadataEditor::gui
