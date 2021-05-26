
namespace bav
{
Parameter::Parameter (RangedParam& p,
                      juce::String paramNameShort,
                      juce::String paramNameVerbose)
    : SerializableData (paramNameVerbose),
      rap (p),
      parameterNameShort (TRANS (paramNameShort)),
      parameterNameVerbose (TRANS (paramNameVerbose))
{
    currentDefault = rap.getDefaultValue();
    lastActionedValue = currentDefault;
    rap.addListener (this);
}

Parameter::~Parameter()
{
    rap.removeListener (this);
}

bool Parameter::operator== (const Parameter& other)
{
    return parameterNameShort == other.parameterNameShort && parameterNameVerbose == other.parameterNameVerbose;
}

float Parameter::getNormalizedDefault() const
{
    return currentDefault;
}

float Parameter::getDenormalizedDefault() const
{
    return denormalize (currentDefault);
}

void Parameter::setNormalizedDefault (float value)
{
    jassert (value >= 0.0f && value <= 1.0f);

    if (currentDefault == value) return;

    currentDefault = value;

    if (onDefaultChange)
        bav::callOnMessageThread (onDefaultChange);
    
    if (um != nullptr)
    {
        
    }
}

void Parameter::setDenormalizedDefault (float value)
{
    setNormalizedDefault (normalize (value));
}

void Parameter::refreshDefault()
{
    currentDefault = getCurrentNormalizedValue();
}

void Parameter::resetToDefault()
{
    rap.setValueNotifyingHost (currentDefault);
}

void Parameter::setNormalizedValue (float value)
{
    rap.setValueNotifyingHost (value);
    
    if (um != nullptr)
    {
        
    }
}

void Parameter::setDenormalizedValue (float value)
{
    setNormalizedValue (normalize (value));
}

float Parameter::getCurrentNormalizedValue() const
{
    return rap.getValue();
}

float Parameter::getCurrentDenormalizedValue() const
{
    return rap.convertFrom0to1 (rap.getValue());
}

float Parameter::normalize (float input) const
{
    return rap.convertTo0to1 (input);
}

float Parameter::denormalize (float input) const
{
    return rap.convertFrom0to1 (input);
}

void Parameter::setUndoManager (juce::UndoManager& managerToUse)
{
    um = &managerToUse;
}

void Parameter::doAction()
{
    const auto value = getCurrentNormalizedValue();

    if (value != lastActionedValue)
    {
        lastActionedValue = value;
        onAction();
    }
}

bool Parameter::isChanging() const
{
    return changing;
}

void Parameter::parameterValueChanged (int, float)
{
    if (onParameterChange)
        bav::callOnMessageThread (onParameterChange);
}

void Parameter::parameterGestureChanged (int, bool gestureIsStarting)
{
    changing = gestureIsStarting;
    if (onGestureStateChange)
        bav::callOnMessageThread< bool > (onGestureStateChange,
                                          gestureIsStarting);
}


}  // namespace bav
