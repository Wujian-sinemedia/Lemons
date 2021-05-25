
namespace bav
{

GainParameter::GainParameter (juce::String                            paramNameShort,
                              juce::String                            paramNameVerbose,
                              juce::AudioProcessorParameter::Category parameterCategory =
                              juce::AudioProcessorParameter::genericParameter)
: FloatParameter (paramNameShort,
                  paramNameVerbose,
                  juce::NormalisableRange< float > (-60.0f, 0.0f, 0.01f),
                  -60.0f,
                  parameterCategory,
                  ParameterValueConversionLambdas::gain_stringFromFloat,
                  ParameterValueConversionLambdas::gain_floatFromString,
                  TRANS ("dB"))
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
