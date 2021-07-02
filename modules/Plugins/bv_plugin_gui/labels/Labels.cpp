
namespace bav::gui
{

StringPropertyLabel::StringPropertyLabel (StringProperty& propertyToUse)
: Label (propertyToUse.getPropertyName()), StringProperty::Listener (propertyToUse), property (propertyToUse)
{
    this->onTextChange = [&]{ property->set (getText()); };
}

void StringPropertyLabel::propertyValueChanged (const String& newValue)
{
    this->set (newValue);
}

}
