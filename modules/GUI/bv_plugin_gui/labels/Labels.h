#pragma once

namespace bav::gui
{

struct StringPropertyLabel : juce::Label, private StringProperty::Listener
{
    StringPropertyLabel (StringProperty& propertyToUse);
    
private:
    void propertyValueChanged (const String& newValue) final;
    
    StringProperty& property;
};

}
