#pragma once

namespace bav::gui
{

struct StringPropertyTextButton : TextButton, private StringProperty::Listener
{
    StringPropertyTextButton (StringProperty& property, std::function< void() > whenClicked);
    
private:
    void propertyValueChanged (const String& newValue) final;
};


class ToggleButton : public juce::ToggleButton,
                     private BoolParameter::Listener
{
public:
    ToggleButton (BoolParameter& paramToUse, std::function< void (bool) > cb = {});

    BoolParameter& param;

private:
    void paramValueChanged (bool newValue) final;
    void refresh();

    std::function< void (bool) > callback;
};

}  // namespace bav::gui
