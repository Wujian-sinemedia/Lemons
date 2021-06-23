
namespace bav::gui
{

DarkModeSentinel::DarkModeSentinel (BoolParameter& paramToUse, juce::Component& componentToUse)
: BoolParameter::Listener (paramToUse),
darkModeParameter (paramToUse),
componentToRepaint (componentToUse)
{
}

void DarkModeSentinel::paramValueChanged (bool)
{
    componentToRepaint.repaint();
}

}
