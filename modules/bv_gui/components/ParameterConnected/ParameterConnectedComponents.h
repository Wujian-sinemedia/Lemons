
#pragma once

namespace bav::gui
{


class SliderAttachmentBase  :   public juce::Slider
{
public:
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
    void paramValueChanged (float newValue) override final;
    void paramDefaultChanged (float newDefault) override final;
    
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
    void paramValueChanged (int newValue) override final;
    void paramDefaultChanged (int newDefault) override final;
    
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
    void paramValueChanged (bool newValue) override final;
};

}  // namespace
