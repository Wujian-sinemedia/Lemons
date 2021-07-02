#pragma once

namespace bav::gui
{

struct StringPropertyLabel : Label, private StringProperty::Listener
{
    StringPropertyLabel (StringProperty& propertyToUse);
    
private:
    void propertyValueChanged (const String& newValue) final;
    
    StringProperty& property;
};

}
