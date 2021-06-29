
namespace bav::gui
{

StringPropertyTextButton::StringPropertyTextButton (StringProperty& property, std::function< void() > whenClicked)
: TextButton (property.get(), whenClicked, property.getPropertyName()), StringProperty::Listener (property)
{
}

void StringPropertyTextButton::propertyValueChanged (const String& newValue)
{
    this->setButtonText (newValue);
    this->setTooltip (newValue);
    this->setName (newValue);
    this->setComponentID (newValue);
}



ToggleButton::ToggleButton (BoolParameter& paramToUse, std::function< void (bool) > cb)
    : BoolParameter::Listener (paramToUse),
      param (paramToUse), callback (std::move (cb))
{
    this->onClick = [&]()
    { refresh(); };
    this->onStateChange = [&]()
    { refresh(); };

    this->setButtonText (param.parameterNameVerbose);
    this->setTooltip (param.parameterNameShort);

    this->setName (param.parameterNameVerbose);
    this->setComponentID (param.parameterNameVerbose);

    param.sendListenerSyncCallback();
}

void ToggleButton::refresh()
{
    param.set (this->getToggleState());
}

void ToggleButton::paramValueChanged (bool newValue)
{
    this->setToggleState (newValue, juce::NotificationType::dontSendNotification);
    callback (newValue);
}

}  // namespace bav::gui
