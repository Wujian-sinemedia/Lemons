#pragma once

namespace bav::gui
{
struct TextButton : public juce::TextButton
{
    TextButton (const String& buttonText, std::function< void() > whenClicked, const String& tooltip = {});
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

    using TB = juce::ToggleButton;
};

}  // namespace bav::gui
