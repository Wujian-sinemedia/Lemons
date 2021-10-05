#pragma once

namespace lemons::plugin
{
class GUIBase : public juce::Component
{
public:
    GUIBase (UndoManager& undoToUse)
        : undoManager (undoToUse)
    {
    }

protected:
    UndoManager& undoManager;

private:
    juce::TooltipWindow tooltipWindow {this, 700};
};


template < typename StateType >
class GUI : public GUIBase
{
public:
    GUI (plugin::PluginState< StateType >& stateToUse)
        : GUIBase (stateToUse.undo), state (stateToUse.state)
    {
    }

protected:
    StateType& state;
};

}  // namespace lemons::plugin
