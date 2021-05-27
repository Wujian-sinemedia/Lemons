
namespace bav::gui
{

SliderWithFloatParam::SliderWithFloatParam (FloatParameter& paramToUse)
: FloatParameter::Listener (paramToUse),
param (paramToUse)
{
    Slider::valueFromTextFunction = [this] (const juce::String& text) { return (double) param.stringToFloat (text); };
    Slider::textFromValueFunction = [this] (double value) { return param.floatToString (float(value), 50); };
    Slider::setDoubleClickReturnValue (true, double (param.getDefault()));
    
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
    
    juce::NormalisableRange<double> newRange { (double) range.start,
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
    
    Slider::setTooltip (param.parameterNameShort);
}

SliderWithFloatParam::~SliderWithFloatParam()
{
}

void SliderWithFloatParam::parameterValueChanged (float newValue)
{
    Slider::setValue (double(newValue), juce::NotificationType::dontSendNotification);
}

void SliderWithFloatParam::parameterDefaultChanged (float newDefault)
{
    Slider::setDoubleClickReturnValue (true, newDefault);
}

void SliderWithFloatParam::startedDragging()
{
    param.beginGesture();
}

void SliderWithFloatParam::stoppedDragging()
{
    param.endGesture();
}

void SliderWithFloatParam::valueChanged()
{
    param.set (float (Slider::getValue()));
}


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/


SliderWithIntParam::SliderWithIntParam (IntParameter& paramToUse)
: IntParameter::Listener (paramToUse),
param (paramToUse)
{
    Slider::valueFromTextFunction = [this] (const juce::String& text) { return (double) param.stringToInt (text); };
    Slider::textFromValueFunction = [this] (double value) { return param.intToString (juce::roundToInt(value), 50); };
    Slider::setDoubleClickReturnValue (true, double(param.getDefault()));
    
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
    
    juce::NormalisableRange<double> newRange { (double) range.start,
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
    
    Slider::setTooltip (param.parameterNameShort);
}

SliderWithIntParam::~SliderWithIntParam()
{
}

void SliderWithIntParam::parameterValueChanged (int newValue)
{
    Slider::setValue (double(newValue), juce::NotificationType::dontSendNotification);
}

void SliderWithIntParam::parameterDefaultChanged (int newDefault)
{
    Slider::setDoubleClickReturnValue (true, newDefault);
}

void SliderWithIntParam::startedDragging()
{
    param.beginGesture();
}

void SliderWithIntParam::stoppedDragging()
{
    param.endGesture();
}

void SliderWithIntParam::valueChanged()
{
    param.set (juce::roundToInt (Slider::getValue()));
}


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/


ToggleButton::ToggleButton (BoolParameter& paramToUse)
: BoolParameter::Listener (paramToUse),
param (paramToUse)
{
    ToggleButton::onClick = [this](){ param.set (Button::getToggleState()); };
    ToggleButton::onStateChange = [this](){ param.set (Button::getToggleState()); };
    
    ToggleButton::setButtonText (param.parameterNameVerbose);
    ToggleButton::setTooltip (param.parameterNameShort);
}

ToggleButton::~ToggleButton()
{
}

void ToggleButton::parameterValueChanged (bool newValue)
{
    ToggleButton::setToggleState (newValue, juce::NotificationType::dontSendNotification);
}

}  // namespace
