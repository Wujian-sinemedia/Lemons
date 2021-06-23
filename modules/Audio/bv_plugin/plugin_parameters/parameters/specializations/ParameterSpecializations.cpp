
namespace bav
{
GainParameter::GainParameter (String                                  paramNameShort,
                              String                                  paramNameVerbose,
                              float                                   defaultVal,
                              juce::AudioProcessorParameter::Category parameterCategory)
    : FloatParameter (paramNameShort,
                      paramNameVerbose,
                      juce::NormalisableRange< float > (-60.0f, 0.0f, 0.01f),
                      defaultVal,
                      parameterCategory,
                      ParameterValueConversionLambdas::gain_stringFromFloat,
                      ParameterValueConversionLambdas::gain_floatFromString,
                      TRANS ("dB"))
{
}


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

ToggleParameter::ToggleParameter (String paramNameShort,
                                  String paramNameVerbose,
                                  bool   defaultVal)
    : BoolParameter (paramNameShort,
                     paramNameVerbose,
                     defaultVal,
                     ParameterValueConversionLambdas::toggle_stringFromBool,
                     ParameterValueConversionLambdas::toggle_boolFromString)
{
}

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

FloatAmountParameter::FloatAmountParameter (String                                  paramNameShort,
                                            String                                  paramNameVerbose,
                                            float                                   defaultVal,
                                            juce::AudioProcessorParameter::Category parameterCategory,
                                            String                                  parameterLabel)
    : FloatParameter (paramNameShort,
                      paramNameVerbose,
                      juce::NormalisableRange< float > (0.0f, 1.0f, 0.01f),
                      defaultVal,
                      parameterCategory,
                      ParameterValueConversionLambdas::normPcnt_stringFromInt,
                      ParameterValueConversionLambdas::normPcnt_intFromString,
                      parameterLabel)
{
}

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

PercentParameter::PercentParameter (String paramNameShort,
                                    String paramNameVerbose,
                                    int    defaultVal)
    : IntParameter (paramNameShort,
                    paramNameVerbose,
                    0,
                    100,
                    defaultVal,
                    ParameterValueConversionLambdas::pcnt_stringFromInt,
                    ParameterValueConversionLambdas::pcnt_intFromString,
                    "%")
{
}

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

MeterParameter::MeterParameter (
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
    String parameterLabel)

    : FloatParameter (paramNameShort,
                      paramNameVerbose,
                      nRange,
                      defaultVal,
                      parameterCategory,
                      stringFromValue,
                      valueFromString,
                      parameterLabel,
                      false)
{
}


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/


GainMeterParameter::GainMeterParameter (String                                  paramNameShort,
                                        String                                  paramNameVerbose,
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

}  // namespace bav
