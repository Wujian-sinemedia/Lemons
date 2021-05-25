
#pragma once

namespace bav
{

struct GainParameter :  public FloatParameter
{
    GainParameter (juce::String                            paramNameShort,
                   juce::String                            paramNameVerbose,
                   float defaultVal,
                   juce::AudioProcessorParameter::Category parameterCategory =
                   juce::AudioProcessorParameter::genericParameter);
    
    virtual ~GainParameter() override = default;
    
    using Listener = FloatParameter::Listener;
};

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

struct ToggleParameter :    public BoolParameter
{
    ToggleParameter (juce::String paramNameShort,
                     juce::String paramNameVerbose,
                     bool         defaultVal);
    
    virtual ~ToggleParameter() override = default;
    
    using Listener = BoolParameter::Listener;
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
    
    virtual ~MeterParameter() override = default;
    
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
    
    virtual ~GainMeterParameter() override = default;
    
    using Listener = FloatParameter::Listener;
};

}  // namespace
