
namespace bav
{
FloatParameter::FloatParameter (
    String paramNameShort,
    String paramNameVerbose,
    juce::NormalisableRange< float >
                                            nRange,
    float                                   defaultVal,
    juce::AudioProcessorParameter::Category parameterCategory,
    std::function< String (float value, int maximumStringLength) >
        stringFromValue,
    std::function< float (const String& text) >
           valueFromString,
    String parameterLabel,
    bool   automatable,
    bool   metaParam)
    : AudioParameterFloat (paramNameVerbose,
                           TRANS (paramNameVerbose),
                           nRange,
                           defaultVal,
                           parameterLabel,
                           parameterCategory,
                           stringFromValue,
                           valueFromString),
      Parameter (*this,
                 paramNameShort,
                 paramNameVerbose,
                 automatable,
                 metaParam),
      floatToString (stringFromValue), stringToFloat (valueFromString)
{
}

bool FloatParameter::isAutomatable() const
{
    return Parameter::isAutomatable;
}

bool FloatParameter::isMetaParameter() const
{
    return Parameter::isMetaParameter;
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
}

void FloatParameter::setDefault (float newDefaultValue)
{
    Parameter::setDenormalizedDefault (newDefaultValue);
}

void FloatParameter::setAction (std::function< void (float) > action)
{
    actionFunc = std::move (action);
}

void FloatParameter::toValueTree (juce::ValueTree& tree)
{
    tree.setProperty ("ParameterValue", get(), nullptr);
    tree.setProperty ("ParameterDefaultValue", getDefault(), nullptr);
    tree.setProperty ("MappedMidiControllerNumber", getMidiControllerNumber(), nullptr);
}

void FloatParameter::fromValueTree (const juce::ValueTree& tree)
{
    set (tree.getProperty ("ParameterValue"));
    setDefault (tree.getProperty ("ParameterDefaultValue"));
    setMidiControllerNumber (tree.getProperty ("MappedMidiControllerNumber"));
}

FloatParameter::Listener::Listener (FloatParameter& toUse)
    : Parameter::Listener (toUse),
      param (toUse)
{
}

void FloatParameter::Listener::parameterValueChanged (float)
{
    paramValueChanged (param.get());
}

void FloatParameter::Listener::parameterDefaultChanged (float)
{
    paramDefaultChanged (param.getDefault());
}

void FloatParameter::Listener::paramValueChanged (float) { }
void FloatParameter::Listener::paramDefaultChanged (float) { }


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/


IntParameter::IntParameter (
    String paramNameShort,
    String paramNameVerbose,
    int    min,
    int    max,
    int    defaultVal,
    std::function< String (int value, int maximumStringLength) >
        stringFromInt,
    std::function< int (const String& text) >
           intFromString,
    String parameterLabel,
    bool   automatable,
    bool   metaParam)
    : AudioParameterInt (paramNameVerbose,
                         TRANS (paramNameVerbose),
                         min,
                         max,
                         defaultVal,
                         parameterLabel,
                         stringFromInt,
                         intFromString),
      Parameter (*this,
                 paramNameShort,
                 paramNameVerbose,
                 automatable,
                 metaParam),
      intToString (stringFromInt), stringToInt (intFromString)
{
}

bool IntParameter::isAutomatable() const
{
    return Parameter::isAutomatable;
}

bool IntParameter::isMetaParameter() const
{
    return Parameter::isMetaParameter;
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
}

void IntParameter::setDefault (int newDefaultValue)
{
    Parameter::setDenormalizedDefault (static_cast< float > (newDefaultValue));
}

void IntParameter::setAction (std::function< void (int) > action)
{
    actionFunc = std::move (action);
}

void IntParameter::toValueTree (juce::ValueTree& tree)
{
    tree.setProperty ("ParameterValue", get(), nullptr);
    tree.setProperty ("ParameterDefaultValue", getDefault(), nullptr);
    tree.setProperty ("MappedMidiControllerNumber", getMidiControllerNumber(), nullptr);
}

void IntParameter::fromValueTree (const juce::ValueTree& tree)
{
    set (tree.getProperty ("ParameterValue"));
    setDefault (tree.getProperty ("ParameterDefaultValue"));
    setMidiControllerNumber (tree.getProperty ("MappedMidiControllerNumber"));
}

IntParameter::Listener::Listener (IntParameter& toUse)
    : Parameter::Listener (toUse),
      param (toUse)
{
}

void IntParameter::Listener::parameterValueChanged (float)
{
    paramValueChanged (param.get());
}

void IntParameter::Listener::parameterDefaultChanged (float)
{
    paramDefaultChanged (param.getDefault());
}

void IntParameter::Listener::paramValueChanged (int) { }
void IntParameter::Listener::paramDefaultChanged (int) { }


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/


BoolParameter::BoolParameter (
    String paramNameShort,
    String paramNameVerbose,
    bool   defaultVal,
    std::function< String (bool value, int maximumStringLength) >
        stringFromBool,
    std::function< bool (const String& text) >
         boolFromString,
    bool automatable,
    bool metaParam)
    : AudioParameterBool (paramNameVerbose,
                          TRANS (paramNameVerbose),
                          defaultVal,
                          juce::String(),
                          stringFromBool,
                          boolFromString),
      Parameter (*this,
                 paramNameShort,
                 paramNameVerbose,
                 automatable,
                 metaParam),
      boolToString (stringFromBool), stringToBool (boolFromString)
{
}

bool BoolParameter::isAutomatable() const
{
    return Parameter::isAutomatable;
}

bool BoolParameter::isMetaParameter() const
{
    return Parameter::isMetaParameter;
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
    if (newValue)
        Parameter::setNormalizedValue (1.0f);
    else
        Parameter::setNormalizedValue (0.0f);
}

void BoolParameter::setDefault (bool newDefaultValue)
{
    if (newDefaultValue)
        Parameter::setNormalizedDefault (1.0f);
    else
        Parameter::setNormalizedDefault (0.0f);
}

void BoolParameter::setAction (std::function< void (bool) > action)
{
    actionFunc = std::move (action);
}

void BoolParameter::toValueTree (juce::ValueTree& tree)
{
    tree.setProperty ("ParameterValue", get(), nullptr);
    tree.setProperty ("ParameterDefaultValue", getDefault(), nullptr);
    tree.setProperty ("MappedMidiControllerNumber", getMidiControllerNumber(), nullptr);
}

void BoolParameter::fromValueTree (const juce::ValueTree& tree)
{
    set (tree.getProperty ("ParameterValue"));
    setDefault (tree.getProperty ("ParameterDefaultValue"));
    setMidiControllerNumber (tree.getProperty ("MappedMidiControllerNumber"));
}

BoolParameter::Listener::Listener (BoolParameter& toUse)
    : Parameter::Listener (toUse),
      param (toUse)
{
}

void BoolParameter::Listener::parameterValueChanged (float)
{
    paramValueChanged (param.get());
}

void BoolParameter::Listener::parameterDefaultChanged (float)
{
    paramDefaultChanged (param.getDefault());
}

void BoolParameter::Listener::paramValueChanged (bool) { }
void BoolParameter::Listener::paramDefaultChanged (bool) { }


}  // namespace bav
