#pragma once

namespace bav::plugin
{
class StateToggleButtons : public juce::Component
{
public:
    StateToggleButtons (StateToggler& togglerToUse);

private:
    void A_ButtonPressed();
    void B_ButtonPressed();
    void C_ButtonPressed();

    StateToggler& toggler;
};

}  // namespace bav::plugin
