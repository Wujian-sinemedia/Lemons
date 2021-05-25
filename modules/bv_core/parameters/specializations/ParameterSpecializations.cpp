
namespace bav
{

FloatParameter::FloatParameter (
                                juce::String paramNameShort,
                                juce::String paramNameVerbose,
                                juce::NormalisableRange< float >
                                nRange,
                                float                                   defaultVal,
                                juce::AudioProcessorParameter::Category parameterCategory,
                                std::function< juce::String (float value, int maximumStringLength) >
                                stringFromValue,
                                std::function< float (const juce::String& text) >
                                valueFromString,
                                juce::String parameterLabel)
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
    return Parameter::getCurrentDenormalizedValue();
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

void FloatParameter::toValueTree (juce::ValueTree& tree)
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
                            std::function< juce::String (int value, int maximumStringLength) >
                            stringFromInt,
                            std::function< int (const juce::String& text) >
                            intFromString,
                            juce::String parameterLabel)
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
    return juce::roundToInt (Parameter::getCurrentDenormalizedValue());
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

void IntParameter::toValueTree (juce::ValueTree& tree)
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
                              std::function< juce::String (bool value, int maximumStringLength) >
                              stringFromBool,
                              std::function< bool (const juce::String& text) >
                              boolFromString)
: AudioParameterBool (paramNameVerbose,
                      TRANS (paramNameVerbose),
                      defaultVal,
                      juce::String(),
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
    return Parameter::getCurrentNormalizedValue() >= 0.5f;
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

void BoolParameter::toValueTree (juce::ValueTree& tree)
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
                                juce::AudioProcessorParameter::Category parameterCategory,
                                std::function< juce::String (float value, int maximumStringLength) >
                                stringFromValue,
                                std::function< float (const juce::String& text) >
                                valueFromString,
                                juce::String parameterLabel)

: FloatParameter (paramNameShort,
                  paramNameVerbose,
                  nRange,
                  defaultVal,
                  parameterCategory,
                  stringFromValue,
                  valueFromString,
                  parameterLabel)
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
                  parameterCategory,
                  ParameterValueConversionLambdas::gain_stringFromFloat,
                  ParameterValueConversionLambdas::gain_floatFromString,
                  TRANS ("dB"))
{
}

}  // namespace
