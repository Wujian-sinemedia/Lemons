
namespace bav
{
Parameter::Parameter (int          keyID,
                      RangedParam& p,
                      float        defaultValue,
                      juce::String paramNameShort,
                      juce::String paramNameVerbose)
    : rap (p),
      key (keyID),
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
        bav::callOnMessageThread< float > (onDefaultChange, value);
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

        if (floatAction)
            floatAction (getFloatValue());
        else if (intAction)
            intAction (getIntValue());
        else if (boolAction)
            boolAction (getBoolValue());
    }
}

void Parameter::setFloatAction (std::function< void (float) > action)
{
    floatAction = std::move (action);
    intAction   = nullptr;
    boolAction  = nullptr;
    voidAction  = nullptr;
}

void Parameter::setIntAction (std::function< void (int) > action)
{
    intAction   = std::move (action);
    floatAction = nullptr;
    boolAction  = nullptr;
    voidAction  = nullptr;
}

void Parameter::setBoolAction (std::function< void (bool) > action)
{
    boolAction  = std::move (action);
    floatAction = nullptr;
    intAction   = nullptr;
    voidAction  = nullptr;
}

void Parameter::setVoidAction (std::function< void() > action)
{
    voidAction  = std::move (action);
    floatAction = nullptr;
    intAction   = nullptr;
    boolAction  = nullptr;
}

bool Parameter::isChanging() const
{
    return changing.load();
}

void Parameter::parameterValueChanged (int, float)
{
    if (onParameterChange)
        bav::callOnMessageThread< float > (onParameterChange,
                                           getCurrentDenormalizedValue());
}

void Parameter::parameterGestureChanged (int, bool gestureIsStarting)
{
    changing.store (gestureIsStarting);
    if (onGestureStateChange)
        bav::callOnMessageThread< bool > (onGestureStateChange,
                                          gestureIsStarting);
}


/*-----------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------*/


FloatParameter::FloatParameter (
    int          keyID,
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
      Parameter (keyID,
                 *this,
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
    Parameter::setFloatAction (action);
}

juce::ValueTree FloatParameter::toValueTree() const
{
    using namespace DefaultValueTreeIds;

    juce::ValueTree tree {Parameter_Float};

    tree.setProperty (ParameterName, Parameter::parameterNameVerbose, nullptr);
    tree.setProperty (ParameterValue, AudioParameterFloat::get(), nullptr);
    tree.setProperty (ParameterDefaultValue, getDefault(), nullptr);
    tree.setProperty (ParameterIsChanging, Parameter::isChanging(), nullptr);

    return tree;
}

void FloatParameter::restoreFromValueTree (const juce::ValueTree& tree)
{
    using namespace DefaultValueTreeIds;
    jassert (tree.hasType (Parameter_Float));

    AudioParameterFloat::setValueNotifyingHost (tree.getProperty (ParameterValue));
    Parameter::setDefault (float (tree.getProperty (ParameterDefaultValue)));
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
    int          keyID,
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
      Parameter (keyID,
                 *this,
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
    Parameter::setIntAction (action);
}

juce::ValueTree IntParameter::toValueTree() const
{
    using namespace DefaultValueTreeIds;

    juce::ValueTree tree {Parameter_Int};

    tree.setProperty (ParameterName, Parameter::parameterNameVerbose, nullptr);
    tree.setProperty (ParameterValue, AudioParameterInt::get(), nullptr);
    tree.setProperty (ParameterDefaultValue, getDefault(), nullptr);
    tree.setProperty (ParameterIsChanging, Parameter::isChanging(), nullptr);

    return tree;
}

void IntParameter::restoreFromValueTree (const juce::ValueTree& tree)
{
    using namespace DefaultValueTreeIds;
    jassert (tree.hasType (Parameter_Int));

    AudioParameterInt::setValueNotifyingHost (tree.getProperty (ParameterValue));
    Parameter::setDefault (int (tree.getProperty (ParameterDefaultValue)));
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
    int          keyID,
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
      Parameter (keyID,
                 *this,
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
    const auto val = newValue ? 1.0f : 0.0f;
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
    Parameter::setBoolAction (action);
}

juce::ValueTree BoolParameter::toValueTree() const
{
    using namespace DefaultValueTreeIds;

    juce::ValueTree tree {Parameter_Bool};

    tree.setProperty (ParameterName, Parameter::parameterNameVerbose, nullptr);
    tree.setProperty (ParameterValue, AudioParameterBool::get(), nullptr);
    tree.setProperty (ParameterDefaultValue, getDefault(), nullptr);
    tree.setProperty (ParameterIsChanging, Parameter::isChanging(), nullptr);

    return tree;
}

void BoolParameter::restoreFromValueTree (const juce::ValueTree& tree)
{
    using namespace DefaultValueTreeIds;
    jassert (tree.hasType (Parameter_Bool));

    AudioParameterBool::setValueNotifyingHost (tree.getProperty (ParameterValue));
    Parameter::setDefault (bool (tree.getProperty (ParameterDefaultValue)));
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
    int          keyID,
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

    : FloatParameter (keyID,
                      paramNameShort,
                      paramNameVerbose,
                      nRange,
                      defaultVal,
                      parameterLabel,
                      parameterCategory,
                      stringFromValue,
                      valueFromString)
{
}

juce::ValueTree MeterParameter::toValueTree() const
{
    juce::ValueTree tree {DefaultValueTreeIds::Parameter_Meter};

    tree.copyPropertiesFrom (FloatParameter::toValueTree(), nullptr);

    return tree;
}

void MeterParameter::restoreFromValueTree (const juce::ValueTree& tree)
{
    using namespace DefaultValueTreeIds;
    jassert (tree.hasType (Parameter_Meter));

    AudioParameterFloat::setValueNotifyingHost (tree.getProperty (ParameterValue));
    Parameter::setDefault (float (tree.getProperty (ParameterDefaultValue)));
}


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/


GainMeterParameter::GainMeterParameter (int                                     keyID,
                                        juce::String                            paramNameShort,
                                        juce::String                            paramNameVerbose,
                                        juce::AudioProcessorParameter::Category parameterCategory)

    : MeterParameter (keyID,
                      paramNameShort,
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
