
#pragma once

namespace bav::gui
{

class SliderWithFloatParam  :   public juce::Slider,
                                private FloatParameter::Listener
{
public:
    SliderWithFloatParam (FloatParameter& paramToUse)
    : param (paramToUse)
    {
        param.addListener (this);
        
        Slider::valueFromTextFunction = [&param] (const String& text) { return (double) param.stringToFloat (text); };
        Slider::textFromValueFunction = [&param] (double value) { return param.floatToString (value, 50); };
        Slider::setDoubleClickReturnValue (true, param.getDefault());
        
        auto range = param.rap.getNormalisableRange();
        
        auto convertFrom0To1Function = [range] (double currentRangeStart,
                                                double currentRangeEnd,
                                                double normalisedValue) mutable
        {
            range.start = (float) currentRangeStart;
            range.end = (float) currentRangeEnd;
            return (double) range.convertFrom0to1 ((float) normalisedValue);
        };
        
        auto convertTo0To1Function = [range] (double currentRangeStart,
                                              double currentRangeEnd,
                                              double mappedValue) mutable
        {
            range.start = (float) currentRangeStart;
            range.end = (float) currentRangeEnd;
            return (double) range.convertTo0to1 ((float) mappedValue);
        };
        
        auto snapToLegalValueFunction = [range] (double currentRangeStart,
                                                 double currentRangeEnd,
                                                 double mappedValue) mutable
        {
            range.start = (float) currentRangeStart;
            range.end = (float) currentRangeEnd;
            return (double) range.snapToLegalValue ((float) mappedValue);
        };
        
        NormalisableRange<double> newRange { (double) range.start,
            (double) range.end,
            std::move (convertFrom0To1Function),
            std::move (convertTo0To1Function),
            std::move (snapToLegalValueFunction) };
        
        newRange.interval = range.interval;
        newRange.skew = range.skew;
        newRange.symmetricSkew = range.symmetricSkew;
        
        Slider::setNormalisableRange (newRange);
        
        Slider::setTextBoxIsEditable (true);
        
        Slider::setTextValueSuffix (param.rap.getLabel());
    }
    
    virtual ~SliderWithFloatParam() override
    {
        param.removeListener (this);
    }
    
    
private:
    void parameterValueChanged (float newValue) override final
    {
        Slider::setValue (double(newValue), juce::NotificationType::dontSendNotification);
    }
    
    void parameterDefaultChanged (float newDefault) override final
    {
        Slider::setDoubleClickReturnValue (true, newDefault);
    }
    
    void startedDragging() override
    {
        param.beginGesture();
    }
    
    void stoppedDragging() override
    {
        param.endGesture();
    }
    
    void valueChanged() override
    {
        param.set (float (Slider::getValue()));
    }
    
    FloatParameter& param;
};

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/
#if 0
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
