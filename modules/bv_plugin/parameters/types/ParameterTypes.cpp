
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
    bool   isAutomatable,
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
                 isAutomatable,
                 metaParam),
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

float FloatParameter::getMinimum() const
{
    return getNormalisableRange().start;
}

float FloatParameter::getMaximum() const
{
    return getNormalisableRange().end;
}

void FloatParameter::set (float newValue)
{
    Parameter::setDenormalizedValue (newValue);
}

void FloatParameter::setDefault (float newDefaultValue)
{
    Parameter::setDenormalizedDefault (newDefaultValue);
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
    bool   isAutomatable,
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
                 isAutomatable,
                 metaParam),
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

int IntParameter::getMinimum() const
{
    return getRange().start;
}

int IntParameter::getMaximum() const
{
    return getRange().end();
}

void IntParameter::set (int newValue)
{
    Parameter::setDenormalizedValue (static_cast< float > (newValue));
}

void IntParameter::setDefault (int newDefaultValue)
{
    Parameter::setDenormalizedDefault (static_cast< float > (newDefaultValue));
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
    bool isAutomatable,
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
                 isAutomatable,
                 metaParam),
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
