
namespace bav
{
Parameter::Parameter (RangedParam& p,
                      float        defaultValue,
                      juce::String paramNameShort,
                      juce::String paramNameVerbose)
    : SerializableData (paramNameVerbose),
      rap (p),
      parameterNameShort (TRANS (paramNameShort)),
      parameterNameVerbose (TRANS (paramNameVerbose))
{
    lastActionedValue = defaultValue;
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

float Parameter::getFloatValue() const
{
    return getCurrentDenormalizedValue();
}

int Parameter::getIntValue() const
{
    return juce::roundToInt (getCurrentDenormalizedValue());
}

bool Parameter::getBoolValue() const
{
    return getCurrentNormalizedValue() >= 0.5f;
}

float Parameter::normalize (float input) const
{
    return rap.convertTo0to1 (input);
}

float Parameter::denormalize (float input) const
{
    return rap.convertFrom0to1 (input);
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


/*-----------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------*/


FloatParameter::FloatParameter (
    juce::String paramNameShort,
    juce::String paramNameVerbose,
    juce::NormalisableRange< float >
                                            nRange,
    float                                   defaultVal,
    juce::String                            parameterLabel,
    juce::AudioProcessorParameter::Category parameterCategory,
    std::function< juce::String (float value, int maximumStringLength) >
        stringFromValue,
    std::function< float (const juce::String& text) >
        valueFromString)
    : AudioParameterFloat (paramNameVerbose,
                           TRANS (paramNameVerbose),
                           nRange,
                           defaultVal,
                           parameterLabel,
                           parameterCategory,
                           stringFromValue,
                           valueFromString),
      Parameter (*this,
                 nRange.convertTo0to1 (defaultVal),
                 paramNameShort,
                 paramNameVerbose),
      floatToString (stringFromValue), stringToFloat (valueFromString)
{
}

float FloatParameter::get() const
{
    return Parameter::getFloatValue();
}

float FloatParameter::getDefault() const
{
    return Parameter::getDenormalizedDefault();
}

void FloatParameter::set (float newValue)
{
    Parameter::setDenormalizedValue (newValue);
    listeners.call ([&newValue] (Listener& l)
                    { l.parameterValueChanged (newValue); });
}

void FloatParameter::setDefault (float newDefaultValue)
{
    Parameter::setDenormalizedDefault (newDefaultValue);
    listeners.call ([&newDefaultValue] (Listener& l)
                    { l.parameterDefaultChanged (newDefaultValue); });
}

void FloatParameter::onGestureChange (bool gestureIsStarting)
{
    listeners.call ([&gestureIsStarting] (Listener& l)
                    { l.parameterGestureStateChanged (gestureIsStarting); });
}

void FloatParameter::setAction (std::function< void (float) > action)
{
    actionFunc = std::move (action);
}

void FloatParameter::onAction()
{
    if (actionFunc)
        actionFunc (get());
}

void FloatParameter::toValueTree (juce::ValueTree& tree) const
{
    tree.setProperty ("ParameterValue", get(), nullptr);
    tree.setProperty ("ParameterDefaultValue", getDefault(), nullptr);
}

void FloatParameter::fromValueTree (const juce::ValueTree& tree)
{
    set (tree.getProperty ("ParameterValue"));
    setDefault (tree.getProperty ("ParameterDefaultValue"));
}

void FloatParameter::addListener (Listener* l)
{
    listeners.add (l);
}

void FloatParameter::removeListener (Listener* l)
{
    listeners.remove (l);
}

void FloatParameter::Listener::parameterValueChanged (float) { }
void FloatParameter::Listener::parameterDefaultChanged (float) { }
void FloatParameter::Listener::parameterGestureStateChanged (bool) { }


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/


IntParameter::IntParameter (
    juce::String paramNameShort,
    juce::String paramNameVerbose,
    int          min,
    int          max,
    int          defaultVal,
    juce::String parameterLabel,
    std::function< juce::String (int value, int maximumStringLength) >
        stringFromInt,
    std::function< int (const juce::String& text) >
        intFromString)
    : AudioParameterInt (paramNameVerbose,
                         TRANS (paramNameVerbose),
                         min,
                         max,
                         defaultVal,
                         parameterLabel,
                         stringFromInt,
                         intFromString),
      Parameter (*this,
                 AudioParameterInt::getNormalisableRange().convertTo0to1 (
                     static_cast< float > (defaultVal)),
                 paramNameShort,
                 paramNameVerbose),
      intToString (stringFromInt), stringToInt (intFromString)
{
}

int IntParameter::get() const
{
    return Parameter::getIntValue();
}

int IntParameter::getDefault() const
{
    return juce::roundToInt (Parameter::getDenormalizedDefault());
}

void IntParameter::set (int newValue)
{
    Parameter::setDenormalizedValue (static_cast< float > (newValue));
    listeners.call ([&newValue] (Listener& l)
                    { l.parameterValueChanged (newValue); });
}

void IntParameter::setDefault (int newDefaultValue)
{
    Parameter::setDenormalizedDefault (static_cast< float > (newDefaultValue));
    listeners.call ([&newDefaultValue] (Listener& l)
                    { l.parameterDefaultChanged (newDefaultValue); });
}

void IntParameter::onGestureChange (bool gestureIsStarting)
{
    listeners.call ([&gestureIsStarting] (Listener& l)
                    { l.parameterGestureStateChanged (gestureIsStarting); });
}

void IntParameter::setAction (std::function< void (int) > action)
{
    actionFunc = std::move (action);
}

void IntParameter::onAction()
{
    if (actionFunc)
        actionFunc (get());
}

void IntParameter::toValueTree (juce::ValueTree& tree) const
{
    tree.setProperty ("ParameterValue", get(), nullptr);
    tree.setProperty ("ParameterDefaultValue", getDefault(), nullptr);
}

void IntParameter::fromValueTree (const juce::ValueTree& tree)
{
    set (tree.getProperty ("ParameterValue"));
    setDefault (tree.getProperty ("ParameterDefaultValue"));
}

void IntParameter::addListener (Listener* l)
{
    listeners.add (l);
}

void IntParameter::removeListener (Listener* l)
{
    listeners.remove (l);
}

void IntParameter::Listener::parameterValueChanged (int) { }
void IntParameter::Listener::parameterDefaultChanged (int) { }
void IntParameter::Listener::parameterGestureStateChanged (bool) { }


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/


BoolParameter::BoolParameter (
    juce::String paramNameShort,
    juce::String paramNameVerbose,
    bool         defaultVal,
    juce::String parameterLabel,
    std::function< juce::String (bool value, int maximumStringLength) >
        stringFromBool,
    std::function< bool (const juce::String& text) >
        boolFromString)
    : AudioParameterBool (paramNameVerbose,
                          TRANS (paramNameVerbose),
                          defaultVal,
                          parameterLabel,
                          stringFromBool,
                          boolFromString),
      Parameter (*this,
                 AudioParameterBool::getNormalisableRange().convertTo0to1 (
                     static_cast< float > (defaultVal)),
                 paramNameShort,
                 paramNameVerbose),
      boolToString (stringFromBool), stringToBool (boolFromString)
{
}

bool BoolParameter::get() const
{
    return Parameter::getBoolValue();
}

bool BoolParameter::getDefault() const
{
    return Parameter::getNormalizedDefault() >= 0.5f;
}

void BoolParameter::set (bool newValue)
{
    const auto val = newValue ? 1.0f : 0.0f;
    Parameter::setNormalizedValue (val);
    listeners.call ([&newValue] (Listener& l)
                    { l.parameterValueChanged (newValue); });
}

void BoolParameter::setDefault (bool newDefaultValue)
{
    const auto val = newDefaultValue ? 1.0f : 0.0f;
    Parameter::setNormalizedDefault (val);
    listeners.call ([&newDefaultValue] (Listener& l)
                    { l.parameterDefaultChanged (newDefaultValue); });
}

void BoolParameter::onGestureChange (bool gestureIsStarting)
{
    listeners.call ([&gestureIsStarting] (Listener& l)
                    { l.parameterGestureStateChanged (gestureIsStarting); });
}

void BoolParameter::setAction (std::function< void (bool) > action)
{
    actionFunc = std::move (action);
}

void BoolParameter::onAction()
{
    if (actionFunc)
        actionFunc (get());
}

void BoolParameter::toValueTree (juce::ValueTree& tree) const
{
    tree.setProperty ("ParameterValue", get(), nullptr);
    tree.setProperty ("ParameterDefaultValue", getDefault(), nullptr);
}

void BoolParameter::fromValueTree (const juce::ValueTree& tree)
{
    set (tree.getProperty ("ParameterValue"));
    setDefault (tree.getProperty ("ParameterDefaultValue"));
}

void BoolParameter::addListener (Listener* l)
{
    listeners.add (l);
}

void BoolParameter::removeListener (Listener* l)
{
    listeners.remove (l);
}

void BoolParameter::Listener::parameterValueChanged (bool) { }
void BoolParameter::Listener::parameterDefaultChanged (bool) { }
void BoolParameter::Listener::parameterGestureStateChanged (bool) { }


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/


MeterParameter::MeterParameter (
    juce::String paramNameShort,
    juce::String paramNameVerbose,
    juce::NormalisableRange< float >
                                            nRange,
    float                                   defaultVal,
    juce::String                            parameterLabel,
    juce::AudioProcessorParameter::Category parameterCategory,
    std::function< juce::String (float value, int maximumStringLength) >
        stringFromValue,
    std::function< float (const juce::String& text) >
        valueFromString)

    : FloatParameter (paramNameShort,
                      paramNameVerbose,
                      nRange,
                      defaultVal,
                      parameterLabel,
                      parameterCategory,
                      stringFromValue,
                      valueFromString)
{
}


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/


GainMeterParameter::GainMeterParameter (juce::String                            paramNameShort,
                                        juce::String                            paramNameVerbose,
                                        juce::AudioProcessorParameter::Category parameterCategory)

    : MeterParameter (paramNameShort,
                      paramNameVerbose,
                      juce::NormalisableRange< float > (-60.0f, 0.0f, 0.01f),
                      -60.0f,
                      TRANS ("dB"),
                      parameterCategory,
                      ParameterValueConversionLambdas::gain_stringFromFloat,
                      ParameterValueConversionLambdas::gain_floatFromString)
{
}


}  // namespace bav
