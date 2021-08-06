#pragma once

namespace bav::plugin
{
class GUI : public juce::Component
{
public:
    GUI (StateToggler& toggler, UndoManager& undo);

protected:
    StateToggler& stateToggler;
    UndoManager&  undoManager;

private:
    juce::TooltipWindow tooltipWindow {this, 700};
};

}  // namespace bav::plugin


#define BV_DECLARE_PLUGIN_GUI_CONSTRUCTOR(Class, StateType) \
    Class (StateType& stateToUse, plugin::StateToggler& toggler, UndoManager& undo)


#define BV_IMPLEMENT_PLUGIN_GUI_CONSTRUCTOR(Class, StateType)                              \
    Class::Class (StateType& stateToUse, plugin::StateToggler& toggler, UndoManager& undo) \
        : plugin::GUI (toggler, undo), state (stateToUse)
