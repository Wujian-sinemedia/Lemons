
namespace lemons::plugin
{
GainParameter::GainParameter (String                                  paramName,
                              float                                   defaultVal,
                              juce::AudioProcessorParameter::Category parameterCategory)
    : FloatParameter (-60.f, 0.f, defaultVal,
                      paramName,
                      ParameterValueConversionLambdas::gain_stringFromFloat,
                      ParameterValueConversionLambdas::gain_floatFromString,
                      TRANS ("dB"), true, false, parameterCategory)
{
}


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

ToggleParameter::ToggleParameter (String paramName,
                                  bool   defaultVal)
    : BoolParameter (defaultVal,
                     paramName,
                     ParameterValueConversionLambdas::toggle_stringFromBool,
                     ParameterValueConversionLambdas::toggle_boolFromString)
{
}

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

FloatAmountParameter::FloatAmountParameter (String                                  paramName,
                                            float                                   defaultVal,
                                            juce::AudioProcessorParameter::Category parameterCategory,
                                            String                                  parameterLabel)
    : FloatParameter (0.f, 1.f, defaultVal,
                      paramName,
                      ParameterValueConversionLambdas::normPcnt_stringFromInt,
                      ParameterValueConversionLambdas::normPcnt_intFromString,
                      parameterLabel, true, false, parameterCategory)
{
}

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

FrequencyParameter::FrequencyParameter (String paramName,
                                        float  defaultVal)
    : FloatParameter (40.f, 10000.f, defaultVal,
                      paramName,
                      ParameterValueConversionLambdas::hz_stringFromFloat,
                      ParameterValueConversionLambdas::hz_floatFromString,
                      TRANS ("Hz"))
{
}

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

PercentParameter::PercentParameter (String paramName,
                                    int    defaultVal)
    : IntParameter (0, 100, defaultVal,
                    paramName,
                    ParameterValueConversionLambdas::pcnt_stringFromInt,
                    ParameterValueConversionLambdas::pcnt_intFromString,
                    "%")
{
}

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

MeterParameter::MeterParameter (float min, float max, float defaultVal,
                                String                                  paramName,
                                std::function< String (float, int) >    stringFromValue,
                                std::function< float (String) >         valueFromString,
                                String                                  parameterLabel,
                                juce::AudioProcessorParameter::Category parameterCategory)

    : FloatParameter (min, max, defaultVal,
                      paramName,
                      stringFromValue,
                      valueFromString,
                      parameterLabel,
                      false, false, parameterCategory)
{
}


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/


GainMeterParameter::GainMeterParameter (String                                  paramName,
                                        juce::AudioProcessorParameter::Category parameterCategory)

    : MeterParameter (-60.f, 0.f, -60.f,
                      paramName,
                      ParameterValueConversionLambdas::gain_stringFromFloat,
                      ParameterValueConversionLambdas::gain_floatFromString,
                      TRANS ("dB"), parameterCategory)
{
}

}  // namespace lemons::plugin
