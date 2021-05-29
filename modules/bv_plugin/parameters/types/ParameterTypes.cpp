
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
    juce::String parameterLabel,
                                bool automatable,
                                bool metaParam)
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

void FloatParameter::onAction()
{
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
    juce::String parameterLabel,
                            bool automatable,
                            bool metaParam)
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

void IntParameter::onAction()
{
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

void BoolParameter::onAction()
{
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


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/
#if 0

ChoiceParameter::ChoiceParameter (juce::String paramNameShort,
                                  juce::String paramNameVerbose,
                                  const juce::StringArray& choices,
                                  int defaultItemIndex,
                                  juce::String parameterLabel)
: AudioParameterChoice (paramNameShort, paramNameVerbose, choices, defaultItemIndex, parameterLabel,
                       [this](int index, int maxLength){ return AudioParameterChoice::choices[index].substring(0, maxLength); },
                       [this](const juce::String& text){ return AudioParameterChoice::choices.indexOf(text, true); }),
Parameter (*this, paramNameShort, paramNameVerbose)
{
    setDefault (AudioParameterChoice::choices[defaultItemIndex]);
}

juce::String ChoiceParameter::get() const
{
    return AudioParameterChoice::getCurrentChoiceName();
}

juce::String ChoiceParameter::getDefault() const
{
    
}

void ChoiceParameter::set (const juce::String& newValue)
{
    const auto index = AudioParameterChoice::choices.indexOf (newValue, true);
    if (index > -1) this = index;
}

void ChoiceParameter::setDefault (const juce::String& newDefaultValue)
{
    
}

void ChoiceParameter::setAction (std::function< void (const juce::String&) > action)
{
    actionFunc = std::move (action);
}

void ChoiceParameter::onAction()
{
    actionFunc (get());
}

void ChoiceParameter::toValueTree (juce::ValueTree& tree)
{
    tree.setProperty ("ParameterValue", get(), nullptr);
    tree.setProperty ("ParameterDefaultValue", getDefault(), nullptr);
}

void ChoiceParameter::fromValueTree (const juce::ValueTree& tree)
{
    set (tree.getProperty ("ParameterValue"));
    setDefault (tree.getProperty ("ParameterDefaultValue"));
}


ChoiceParameter::Listener::Listener (BoolParameter& toUse)
: Parameter::Listener (toUse),
param (toUse)
{
}

void ChoiceParameter::Listener::parameterValueChanged (float)
{
    paramValueChanged (param.get());
}

void ChoiceParameter::Listener::parameterDefaultChanged (float)
{
    paramDefaultChanged (param.getDefault());
}

void ChoiceParameter::Listener::paramValueChanged (const juce::String&) { }
void ChoiceParameter::Listener::paramDefaultChanged (const juce::String&) { }

#endif


}  // namespace bav
