
#pragma once

namespace bav::gui
{
class SliderAttachmentBase : public juce::Slider
{
public:
    SliderAttachmentBase (Parameter& paramToUse);
    virtual ~SliderAttachmentBase() override = default;

private:
    Parameter& param;

    void startedDragging() final;
    void stoppedDragging() final;
};


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/


class FloatSlider : public SliderAttachmentBase,
                    private bav::FloatParameter::Listener
{
public:
    FloatSlider (FloatParameter& paramToUse);
    FloatSlider (FloatParameter& paramToUse, SliderStyle style);

    virtual ~FloatSlider() override = default;

    FloatParameter& param;

private:
    void paramValueChanged (float newValue) final;
    void paramDefaultChanged (float newDefault) final;

    void valueChanged() final;
};


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/


class IntSlider : public SliderAttachmentBase,
                  private bav::IntParameter::Listener
{
public:
    IntSlider (IntParameter& paramToUse);
    IntSlider (IntParameter& paramToUse, SliderStyle style);

    virtual ~IntSlider() override = default;

    IntParameter& param;

private:
    void paramValueChanged (int newValue) final;
    void paramDefaultChanged (int newDefault) final;

    void valueChanged() final;
};


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/


class ToggleButton : public juce::ToggleButton,
                     private bav::BoolParameter::Listener
{
public:
    ToggleButton (BoolParameter& paramToUse);
    virtual ~ToggleButton() override;

    BoolParameter& param;

private:
    void paramValueChanged (bool newValue) final;
    void refresh();

    using TB = juce::ToggleButton;
};

}  // namespace bav::gui
