#pragma once

namespace lemons::plugin
{
/** Base class for all plugin GUI's.
    @see GUI
 */
class GUIBase : public juce::Component
{
public:
    /** We need non-templated access to Toggler here... */
    using Toggle = serializing::TogglerInterface;

    /** Creates a new plugin GUI. */
    GUIBase (UndoManager& undoToUse, StateBase& stateBaseToUse, Toggle& toggleInterfaceToUse)
        : undoManager (undoToUse), stateBase (stateBaseToUse), toggleInterface (toggleInterfaceToUse)
    {
    }

protected:
    /** The plugin's UndoManager. */
    UndoManager& undoManager;

    /** The plugin's state. */
    StateBase& stateBase;

    /** The plugin's state toggles. */
    Toggle& toggleInterface;

    /** The plugin's tooltip window. */
    juce::TooltipWindow tooltipWindow {this, 700};
};


/** Base class for plugin GUI's.
    Your plugin's main component should inherit from this.
    @tparam StateType The type of your plugin's main state object. This should be the same StateType that you sent to Processor and PluginState's template arguments.
    @see GUIBase
 */
template < typename StateType >
class GUI : public GUIBase
{
public:
    /** Creates a new plugin GUI. */
    GUI (plugin::PluginState< StateType >& stateToUse)
        : GUIBase (stateToUse.undo, stateToUse.state, stateToUse.toggles),
          state (stateToUse.state), toggles (stateToUse.toggles)
    {
    }

protected:
    /** The plugin's main state object. */
    StateType&    state;
    StateToggles& toggles;
};

}  // namespace lemons::plugin
