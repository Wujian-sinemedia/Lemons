#pragma once

namespace lemons::plugin
{
/** Base class for all plugin GUI's.
    @see GUI
 */
class GUIBase : public juce::Component
{
public:
	/** Creates a new plugin GUI. */
	GUIBase (StateBase& stateBaseToUse)  //, Toggle& toggleInterfaceToUse)
	    : stateBase (stateBaseToUse)
	{
	}

protected:
	/** The plugin's state. */
	StateBase& stateBase;

	/** The plugin's tooltip window. */
	juce::TooltipWindow tooltipWindow { this, 700 };
};


/** Base class for plugin GUI's.
    Your plugin's main component should inherit from this.
    @tparam StateType The type of your plugin's main state object. This should be the same StateType that you sent to Processor and PluginState's template arguments.
    @see GUIBase
 */
template <typename StateType>
class GUI : public GUIBase
{
public:
	/** Creates a new plugin GUI. */
	GUI (StateType& stateToUse)
	    : GUIBase (stateToUse.state, stateToUse.toggles)
	    , state (stateToUse.state)
	{
	}

protected:
	/** The plugin's main state object. */
	StateType& state;
};

}  // namespace lemons::plugin
