
namespace bav::gui
{

StringPropertyTextButton::StringPropertyTextButton (StringProperty& property, std::function< void() > whenClicked)
: TextButton (property.get(), whenClicked), StringProperty::Listener (property)
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
: ToggleTextButton (paramToUse.parameterNameVerbose, [&](bool state){ clicked_callback (state); }),
      param (paramToUse), callback (std::move (cb))
{
}

void ToggleButton::clicked_callback (bool state)
{
    param.set (state);
    callback (state);
}

}  // namespace bav::gui
