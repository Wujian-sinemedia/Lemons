
namespace bav
{

Parameter::Parameter    (int          keyID,
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
    changing.store (false);
}


Parameter::~Parameter()
{
    rap.removeListener (this);
}

float Parameter::getNormalizedDefault() const
{
    return currentDefault.load();
}

float Parameter::getDenormalizedDefault() const
{
    return denormalize (currentDefault.load());
}

void Parameter::setNormalizedDefault (float value)
{
    jassert (value >= 0.0f && value <= 1.0f);
    
    if (currentDefault.load() == value) return;
    
    currentDefault.store (value);
    
    if (onDefaultChange)
        bav::callOnMessageThread< float > (onDefaultChange, value);
}

void Parameter::setDefault (float value)
{
    setNormalizedDefault (normalize (value)); 
}

void Parameter::setDefault (int value)
{
    setNormalizedDefault (normalize (static_cast< float > (value)));
}

void Parameter::setDefault (bool value)
{
    const auto def = value ? 1.0f : 0.0f;
    setNormalizedDefault (def);
}

void Parameter::refreshDefault()
{
    currentDefault.store (getCurrentNormalizedValue());
}

void Parameter::resetToDefault()
{
    rap.setValueNotifyingHost (currentDefault.load());
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

float Parameter::normalize (int input) const
{
    return rap.convertTo0to1 (static_cast<float> (input));
}

float Parameter::normalize (bool input) const
{
    return input ? 1.0f : 0.0f;
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
        else if (voidAction)
            voidAction();
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
                std::function< float (const juce::String& text) > valueFromString)
: AudioParameterFloat (paramNameVerbose,
                       TRANS (paramNameVerbose),
                       nRange,
                       defaultVal,
                       parameterLabel,
                       parameterCategory,
                       stringFromValue,
                       valueFromString),
Parameter (key,
           *this,
           nRange.convertTo0to1 (defaultVal),
           paramNameShort,
           paramNameVerbose),
floatToString (stringFromValue), stringToFloat (valueFromString)
{
}

float FloatParameter::getDefault() const
{
    return Parameter::getDenormalizedDefault();
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
              std::function< int (const juce::String& text) > intFromString)
: AudioParameterInt (paramNameVerbose,
                     TRANS (paramNameVerbose),
                     min,
                     max,
                     defaultVal,
                     parameterLabel,
                     stringFromInt,
                     intFromString),
Parameter (key,
           *this,
           AudioParameterInt::getNormalisableRange().convertTo0to1 (
                                                                    static_cast< float > (defaultVal)),
           paramNameShort,
           paramNameVerbose),
intToString (stringFromInt), stringToInt (intFromString)
{
}

int IntParameter::getDefault() const
{
    return juce::roundToInt (Parameter::getDenormalizedDefault());
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
               std::function< bool (const juce::String& text) > boolFromString)
: AudioParameterBool (paramNameVerbose,
                      TRANS (paramNameVerbose),
                      defaultVal,
                      parameterLabel,
                      stringFromBool,
                      boolFromString),
Parameter (key,
           *this,
           AudioParameterBool::getNormalisableRange().convertTo0to1 (
                                                                     static_cast< float > (defaultVal)),
           paramNameShort,
           paramNameVerbose),
boolToString (stringFromBool), stringToBool (boolFromString)
{
}

bool BoolParameter::getDefault() const
{
    return Parameter::getNormalizedDefault() >= 0.5f;
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
                std::function< float (const juce::String& text) > valueFromString)

: FloatParameter (key,
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

: MeterParameter (key,
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


}  // namespace
