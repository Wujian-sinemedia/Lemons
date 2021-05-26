
#pragma once

namespace bav::gui
{
#if 0

class SliderWithFloatParam  :   public juce::Slider
{
public:
    SliderWithFloatParam (FloatParameter& paramToUse)
    : param (paramToUse)
    { }
    
    virtual ~SliderWithFloatParam() = default;
    
private:
    FloatParameter& param;
};

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

class SliderWithIntParam    :   public juce::Slider
{
public:
    SliderWithIntParam (IntParameter& paramToUse)
    : param (paramToUse)
    { }
    
    virtual ~SliderWithIntParam() = default;
    
private:
    IntParameter& param;
};

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

class ToggleButton      :   public juce::ToggleButton
{
public:
    ToggleButton (BoolParameter& paramToUse)
    : param (paramToUse)
    { }
    
    virtual ~ToggleButton() = default;
    
private:
    BoolParameter& param;
};

#endif  // if 0
}  // namespace
