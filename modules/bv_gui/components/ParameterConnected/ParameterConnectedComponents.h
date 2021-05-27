
#pragma once

namespace bav::gui
{

class SliderWithFloatParam  :   public juce::Slider,
                                private bav::FloatParameter::Listener
{
public:
    SliderWithFloatParam (FloatParameter& paramToUse);
    
    virtual ~SliderWithFloatParam() override;
    
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

class SliderWithIntParam  :   public juce::Slider,
                              private bav::IntParameter::Listener
{
public:
    SliderWithIntParam (IntParameter& paramToUse);
    
    virtual ~SliderWithIntParam() override;
    
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
