#pragma once

namespace lemons::gui
{

/**
    A stateful on/off button attached to a boolean plugin parameter.
    For a simple press-button that triggers a lambda function, use TextButton or ImageButton.
    @see plugin::BoolParameter
 */
class ToggleButton : public ToggleTextButton
{
public:
    /**
        Creates a ToggleButton linked to a boolean parameter.
        @param paramToUse The parameter to attach to.
        @param cb A lambda function that will be called when the state of the button/parameter changes.
     */
    ToggleButton (plugin::BoolParameter& paramToUse, std::function< void (bool) > cb = {});

    /** Destructor. */
    virtual ~ToggleButton() = default;

    plugin::BoolParameter& param;

private:
    void clicked_callback (bool state);

    std::function< void (bool) > callback;
};

}  // namespace lemons::gui
