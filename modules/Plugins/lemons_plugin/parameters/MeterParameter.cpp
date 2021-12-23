/*
 ======================================================================================

 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.

 ======================================================================================
 */

namespace lemons::plugin
{

MeterParameter::MeterParameter (float min, float max, float defaultVal,
                                String                                  paramName,
                                std::function<String (float, int)>      stringFromValue,
                                std::function<float (String)>           valueFromString,
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
                      gain_stringFromFloat,
                      gain_floatFromString,
                      TRANS ("dB"), parameterCategory)
{
}

}  // namespace lemons::plugin
