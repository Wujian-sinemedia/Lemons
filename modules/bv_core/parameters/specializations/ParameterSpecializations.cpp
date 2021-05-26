
namespace bav
{

GainParameter::GainParameter (juce::String                            paramNameShort,
                              juce::String                            paramNameVerbose,
                              float defaultVal,
                              juce::AudioProcessorParameter::Category parameterCategory)
: FloatParameter (paramNameShort,
                  paramNameVerbose,
                  juce::NormalisableRange< float > (-60.0f, 0.0f, 0.01f),
                  defaultVal,
                  parameterCategory,
                  ParameterValueConversionLambdas::gain_stringFromFloat,
                  ParameterValueConversionLambdas::gain_floatFromString,
                  TRANS ("dB"))
{ }


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

ToggleParameter::ToggleParameter (juce::String paramNameShort,
                                  juce::String paramNameVerbose,
                                  bool         defaultVal)
: BoolParameter (paramNameShort,
                 paramNameVerbose,
                 defaultVal,
                 ParameterValueConversionLambdas::toggle_stringFromBool,
                 ParameterValueConversionLambdas::toggle_boolFromString)
{ }

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

FloatAmountParameter::FloatAmountParameter (juce::String paramNameShort,
                                            juce::String paramNameVerbose,
                                            float        defaultVal,
                                            juce::AudioProcessorParameter::Category parameterCategory,
                                            juce::String parameterLabel)
: FloatParameter (paramNameShort,
                  paramNameVerbose,
                  juce::NormalisableRange< float > (0.0f, 1.0f, 0.01f),
                  defaultVal,
                  parameterCategory,
                  ParameterValueConversionLambdas::normPcnt_stringFromInt,
                  ParameterValueConversionLambdas::normPcnt_intFromString,
                  parameterLabel)
{ }

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

PercentParameter::PercentParameter (juce::String paramNameShort,
                                    juce::String paramNameVerbose,
                                    int defaultVal)
: IntParameter (paramNameShort,
                paramNameVerbose,
                0,
                100,
                defaultVal,
                ParameterValueConversionLambdas::pcnt_intFromString,
                ParameterValueConversionLambdas::pcnt_stringFromInt,
                "%")
{ }

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
