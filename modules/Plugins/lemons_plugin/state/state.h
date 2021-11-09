#pragma once

#include <lemons_plugin_parameters/lemons_plugin_parameters.h>

namespace lemons::plugin
{
/** Represent the dimensions of a plugin editor using a juce::Point<int>. */
using Dimensions = juce::Point<int>;


/** The default plugin state toggler has 3 states, A/B/C. */
//using StateToggles = serializing::Toggler<3>;


/** Base class for a plugin's state.
    This class references a ParameterList that you give it; you can also specify another SerializableData object to be serialized with your plugin state. This class internally holds a Dimensions (juce::Point representing the plugin editor's size), and a ModulationManager object that will reference the plugin's ParameterList. \n
    \n
    This class will automatically add a master bypass ToggleParam to your parameter list.
    \n
    I recommed using the templated State or CustomState classes; they handle creating your parameter list and custom serializable data objects and passing them to the base class.
    @see State, CustomState, PluginState, ParameterList, ModulationManager, Dimensions, StateToggler
 */
class StateBase
{
public:
	/** Creates a new plugin state.
	    @param pluginName The name of this plugin.
	    @param paramsToUse The plugin's parameter list.
	    @param customStateDataToUse An optional additional serializable object to save with your plugin's state.
	 */
	StateBase (const String& pluginName);


	/** Adds the state's parameters to the specified AudioProcessor.
	    @see ParameterList::addParametersTo()
	 */
	void addTo (juce::AudioProcessor& processor);

	/** Adds all the state's parameters to a dummy internal processor.
	    @see ParameterList::addAllParametersAsInternal()
	 */
	void addAllAsInternal();

	/** The plugin's main bypass parameter. */
	ToggleParam mainBypass { "Main bypass", false };

	/** The plugin editor's dimensions. */
	Dimensions dimensions;

	/** The plugin's modulation manager. */
	ModulationManager modManager;
};

}  // namespace lemons::plugin
