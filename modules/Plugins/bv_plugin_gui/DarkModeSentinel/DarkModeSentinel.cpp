
namespace bav::gui
{

DarkModeSentinel::DarkModeSentinel (plugin::BoolParameter& paramToUse, juce::Component& componentToUse)
: plugin::BoolParameter::Listener (paramToUse),
darkModeParameter (paramToUse),
componentToRepaint (componentToUse)
{
}

void DarkModeSentinel::paramValueChanged (bool)
{
    componentToRepaint.repaint();
}

}
