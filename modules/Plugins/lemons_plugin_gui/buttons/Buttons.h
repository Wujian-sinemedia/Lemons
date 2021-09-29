#pragma once

namespace lemons::gui
{
class ToggleButton : public ToggleTextButton
{
public:
    ToggleButton (plugin::BoolParameter& paramToUse, std::function< void (bool) > cb = {});

    virtual ~ToggleButton() = default;

    plugin::BoolParameter& param;

private:
    void clicked_callback (bool state);

    std::function< void (bool) > callback;
};

}  // namespace lemons::gui
