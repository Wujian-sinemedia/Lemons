
#pragma once

namespace bav::gui
{


class SliderAttachmentBase  :   public juce::Slider
{
public:
    SliderAttachmentBase (Parameter& paramToUse);
    virtual ~SliderAttachmentBase() override = default;
    
private:
    Parameter& param;
    
    void startedDragging() override final;
    void stoppedDragging() override final;
};


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/


class FloatSlider  :   public SliderAttachmentBase,
                       private bav::FloatParameter::Listener
{
public:
    using SliderStyle = juce::SliderStyle;
    
    FloatSlider (FloatParameter& paramToUse);
    FloatSlider (FloatParameter& paramToUse, SliderStyle style);
    
    virtual ~FloatSlider() override = default;
    
    FloatParameter& param;
    
private:
    void paramValueChanged (float newValue) override final;
    void paramDefaultChanged (float newDefault) override final;
    
    void valueChanged() override final;
};


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/


class IntSlider  :   public SliderAttachmentBase,
                     private bav::IntParameter::Listener
{
public:
    IntSlider (IntParameter& paramToUse);
    virtual ~IntSlider() override = default;
    
    IntParameter& param;
    
private:
    void paramValueChanged (int newValue) override final;
    void paramDefaultChanged (int newDefault) override final;
    
    void valueChanged() override final;
};


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/


class ToggleButton      :   public juce::ToggleButton,
                            private bav::BoolParameter::Listener
{
public:
    ToggleButton (BoolParameter& paramToUse);
    virtual ~ToggleButton() override;
    
    BoolParameter& param;
    
private:
    void paramValueChanged (bool newValue) override final;
    void refresh();
};

}  // namespace
