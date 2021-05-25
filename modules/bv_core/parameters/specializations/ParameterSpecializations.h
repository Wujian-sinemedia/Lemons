
#pragma once

namespace bav
{

struct GainParameter :  public FloatParameter
{
    GainParameter (juce::String                            paramNameShort,
                   juce::String                            paramNameVerbose,
                   juce::AudioProcessorParameter::Category parameterCategory =
                   juce::AudioProcessorParameter::genericParameter);
    
    using Listener = FloatParameter::Listener;
};

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

struct MeterParameter : public FloatParameter
{
    MeterParameter (
                    juce::String paramNameShort,
                    juce::String paramNameVerbose,
                    juce::NormalisableRange< float >
                    nRange,
                    float                                   defaultVal,
                    juce::AudioProcessorParameter::Category parameterCategory =
                    juce::AudioProcessorParameter::genericParameter,
                    std::function< juce::String (float value, int maximumStringLength) >
                    stringFromValue = nullptr,
                    std::function< float (const juce::String& text) > valueFromString = nullptr,
                    juce::String parameterLabel = juce::String());
    
    bool isAutomatable() const override final { return false; }
    
    using Listener = FloatParameter::Listener;
};


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/


struct GainMeterParameter : public MeterParameter
{
    GainMeterParameter (juce::String                            paramNameShort,
                        juce::String                            paramNameVerbose,
                        juce::AudioProcessorParameter::Category parameterCategory =
                        juce::AudioProcessorParameter::genericParameter);
    
    using Listener = FloatParameter::Listener;
};

}  // namespace
