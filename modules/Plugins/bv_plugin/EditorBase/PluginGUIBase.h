#pragma once

namespace bav::plugin
{
class GUIBase : public juce::Component
{
public:
    GUIBase (StateToggler& toggler, UndoManager& undo);

    template < typename StateType >
    GUIBase (plugin::PluginState< StateType >& state)
        : GUIBase (state.toggles, state.undo)
    {
    }

protected:
    StateToggler& stateToggler;
    UndoManager&  undoManager;

private:
    juce::TooltipWindow tooltipWindow {this, 700};
};


template < typename StateType >
class GUI : public GUIBase
{
public:
    GUI (plugin::PluginState< StateType >& stateToUse)
        : GUIBase (stateToUse), state (stateToUse.state)
    {
    }

protected:
    StateType& state;
};

}  // namespace bav::plugin
