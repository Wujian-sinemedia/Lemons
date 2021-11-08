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
	StateBase (const String& pluginName, ParameterList& paramsToUse, SerializableData* customStateDataToUse = nullptr);


	/** Adds the state's parameters to the specified AudioProcessor.
	    @see ParameterList::addParametersTo()
	 */
	void addTo (juce::AudioProcessor& processor);

	/** Adds all the state's parameters to a dummy internal processor.
	    @see ParameterList::addAllParametersAsInternal()
	 */
	void addAllAsInternal();

	/** Returns a reference to the plugin's ParameterList. */
	[[nodiscard]] ParameterList& getParameters() noexcept;

	/** The plugin's main bypass parameter. */
	ToggleParam mainBypass { "Main bypass", false };

	/** The plugin editor's dimensions. */
	Dimensions dimensions;

	/** The plugin's modulation manager. */
	ModulationManager modManager;

protected:
	void serialize (TreeReflector& ref) final;

	ParameterList& params;

	SerializableData* customStateData { nullptr };
};


/** A plugin state class that instantiates the appropriate ParameterList and passes it to the StateBase class for you.
    To use this with a custom serializable state object as well, see CustomState. \n
    Example usage:
    @code
    using namespace lemons;

    // define our parameter list type for our plugin:
    struct MyParams : plugin::ParameterList
    {
        MyParams()
        {
            add (reverbToggle);
        }

        ToggleParam reverbToggle {"Reverb toggle", true};
    };

    // now this is our full state object:
    using MyPluginState = plugin::State< MyParams >;
    @endcode
    @tparam ParamListType The type of parameter list to use for your plugin. This type must inherit from ParameterList and must be default-constructable.
    @see CustomState, PluginState, StateBase, ParameterList
 */
template <typename ParamListType,
          LEMONS_MUST_INHERIT_FROM (ParamListType, ParameterList)>
struct State : StateBase
{
	/** Creates a new plugin state.
	    @param pluginName The name of this plugin.
	 */
	State (String pluginName)
	    : StateBase (pluginName, parameters)
	{
	}

	/** The plugin's ParameterList. */
	ParamListType parameters;
};


/** A plugin state class that instantiates the appropriate ParameterList and custom serializable object and passes them to the StateBase class for you.
    If you don't need a custom serializable object, see the State class. \n
    Example usage:
    @code
    using namespace lemons;

    // define our parameter list type for our plugin:
    struct MyParams : plugin::ParameterList
    {
        MyParams()
        {
            add (reverbToggle);
        }

        ToggleParam reverbToggle {"Reverb toggle", true};
    };

    // define our custom serializable data for our plugin:
    struct MyCustomData : SerializableData
    {
        String username;
        std::vector<int> points;

    private:
        void serialize (TreeReflector& ref) final
        {
            ref.add ("Username", username);
            ref.add ("Point", points);
        }
    };

    // now, this is our plugin's full state object:
    using MyPluginState = plugin::CustomState< MyParams, MyCustomData >;
    @endcode
    @tparam ParamListType The type of parameter list to use for your plugin. This type must inherit from ParameterList and must be default-constructable.
    @tparam CustomDataType The type of custom serializable object to use for your plugin. This type must inherit from SerializableData and must be default-constructable.
    @see State, PluginState, StateBase, ParameterList
 */
template <typename ParamListType, typename CustomDataType,
          LEMONS_MUST_INHERIT_FROM (ParamListType, ParameterList),
          LEMONS_MUST_INHERIT_FROM (CustomDataType, SerializableData)>
struct CustomState : StateBase
{
	/** Creates a new plugin state.
	    @param pluginName The name of this plugin.
	 */
	CustomState (String pluginName)
	    : StateBase (pluginName, parameters, &customData)
	{
	}

	/** The plugin's ParameterList. */
	ParamListType parameters;

	/** The plugin's custom SerializableData object. */
	CustomDataType customData;
};


}  // namespace lemons::plugin
