
#pragma once

namespace bav::gui
{


class SliderAttachmentBase  :   public juce::Slider
{
    SliderAttachmentBase (juce::RangedAudioParameter& paramToUse);
    
    virtual ~SliderAttachmentBase() override = default;
    
private:
    juce::RangedAudioParameter& param;
};


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/


class FloatSlider  :   public SliderAttachmentBase,
                       private bav::FloatParameter::Listener
{
public:
    FloatSlider (FloatParameter& paramToUse);
    
    virtual ~FloatSlider() override = default;
    
    FloatParameter& param;
    
private:
    void parameterValueChanged (float newValue) override final;
    void parameterDefaultChanged (float newDefault) override final;
    
    void startedDragging() override final;
    void stoppedDragging() override final;
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
    void parameterValueChanged (int newValue) override final;
    void parameterDefaultChanged (int newDefault) override final;
    
    void startedDragging() override final;
    void stoppedDragging() override final;
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
    void parameterValueChanged (bool newValue) override final;
};

}  // namespace
