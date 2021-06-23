
namespace bav::gui
{
TextButton::TextButton (const String& buttonText, std::function< void() > whenClicked, const String& tooltip)
    : juce::TextButton (buttonText, tooltip)
{
    onClick = std::move (whenClicked);
}


ToggleButton::ToggleButton (BoolParameter& paramToUse, std::function< void (bool) > cb)
    : BoolParameter::Listener (paramToUse),
      param (paramToUse), callback (std::move (cb))
{
    TB::onClick = [&]()
    { refresh(); };
    TB::onStateChange = [&]()
    { refresh(); };

    TB::setButtonText (param.parameterNameVerbose);
    TB::setTooltip (param.parameterNameShort);

    TB::setName (param.parameterNameVerbose);
    TB::setComponentID (param.parameterNameVerbose);

    param.sendListenerSyncCallback();
}

void ToggleButton::refresh()
{
    param.set (TB::getToggleState());
}

void ToggleButton::paramValueChanged (bool newValue)
{
    TB::setToggleState (newValue, juce::NotificationType::dontSendNotification);
    callback (newValue);
}

}  // namespace bav::gui
