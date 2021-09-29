#pragma once

#include <lemons_plugin/lemons_plugin.h>


namespace lemons::plugin
{
class StateToggleButtons : public juce::Component
{
public:
    StateToggleButtons (StateToggler& togglerToUse);

    bool isVertical() const;
    void setVertical (bool shouldBeVertical);

private:
    void resized() final;

    void A_ButtonPressed();
    void B_ButtonPressed();
    void C_ButtonPressed();

    StateToggler& toggler;

    gui::TextButton a_button {"A", [this]
                              { A_ButtonPressed(); }};
    gui::TextButton b_button {"B", [this]
                              { B_ButtonPressed(); }};
    gui::TextButton c_button {"C", [this]
                              { C_ButtonPressed(); }};

    bool vertical {false};
};

}  // namespace bav::plugin
