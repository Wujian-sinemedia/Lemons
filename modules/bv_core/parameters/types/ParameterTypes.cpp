
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
    if (newValue == get())
        return;

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

FloatParameter::Listener::Listener (FloatParameter& toUse)
    : Parameter::Listener (toUse),
      param (toUse)
{
    param.addParameterListener (this);
}

FloatParameter::Listener::~Listener()
{
    param.removeParameterListener (this);
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
    if (newValue == get())
        return;

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

IntParameter::Listener::Listener (IntParameter& toUse)
    : Parameter::Listener (toUse),
      param (toUse)
{
    param.addParameterListener (this);
}

IntParameter::Listener::~Listener()
{
    param.removeParameterListener (this);
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
    if (newValue == get())
        return;

    const auto val = newValue ? 1.0f : 0.0f;
    Parameter::setNormalizedValue (val);
}

void BoolParameter::setDefault (bool newDefaultValue)
{
    const auto val = newDefaultValue ? 1.0f : 0.0f;
    Parameter::setNormalizedDefault (val);
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

BoolParameter::Listener::Listener (BoolParameter& toUse)
    : Parameter::Listener (toUse),
      param (toUse)
{
    param.addParameterListener (this);
}

BoolParameter::Listener::~Listener()
{
    param.removeParameterListener (this);
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
