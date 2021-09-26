#pragma once

namespace bav::plugin
{
class GUIBase : public juce::Component
{
public:
    GUIBase (StateToggler& togglerToUse, UndoManager& undoToUse)
        : stateToggler (togglerToUse), undoManager (undoToUse)
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
        : GUIBase (stateToUse.toggles, stateToUse.undo), state (stateToUse.state)
    {
    }

protected:
    StateType& state;
};

}  // namespace bav::plugin
