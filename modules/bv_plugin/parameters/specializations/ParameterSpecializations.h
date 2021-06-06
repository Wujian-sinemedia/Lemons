
#pragma once

namespace bav
{
struct GainParameter : public FloatParameter
{
    GainParameter (String                                  paramNameShort,
                   String                                  paramNameVerbose,
                   float                                   defaultVal,
                   juce::AudioProcessorParameter::Category parameterCategory =
                       juce::AudioProcessorParameter::genericParameter);

    virtual ~GainParameter() override = default;

    using Listener = FloatParameter::Listener;
};

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

struct ToggleParameter : public BoolParameter
{
    ToggleParameter (String paramNameShort,
                     String paramNameVerbose,
                     bool   defaultVal);

    virtual ~ToggleParameter() override = default;

    using Listener = BoolParameter::Listener;
};

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

/* This parameter is for a value in a normalized 0.0-1.0 range */
struct FloatAmountParameter : public FloatParameter
{
    FloatAmountParameter (String                                  paramNameShort,
                          String                                  paramNameVerbose,
                          float                                   defaultVal,
                          juce::AudioProcessorParameter::Category parameterCategory =
                              juce::AudioProcessorParameter::genericParameter,
                          String parameterLabel = String());

    virtual ~FloatAmountParameter() override = default;

    using Listener = FloatParameter::Listener;
};

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

/* This parameter is for an integer value in the range 0 - 100 */
struct PercentParameter : public IntParameter
{
    PercentParameter (String paramNameShort,
                      String paramNameVerbose,
                      int    defaultVal);

    virtual ~PercentParameter() override = default;

    using Listener = IntParameter::Listener;
};

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

struct MeterParameter : public FloatParameter
{
    MeterParameter (
        String paramNameShort,
        String paramNameVerbose,
        juce::NormalisableRange< float >
                                                nRange,
        float                                   defaultVal,
        juce::AudioProcessorParameter::Category parameterCategory =
            juce::AudioProcessorParameter::genericParameter,
        std::function< String (float value, int maximumStringLength) >
                                                    stringFromValue = nullptr,
        std::function< float (const String& text) > valueFromString = nullptr,
        String                                      parameterLabel  = String());

    virtual ~MeterParameter() override = default;

    using Listener = FloatParameter::Listener;
};


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/


struct GainMeterParameter : public MeterParameter
{
    GainMeterParameter (String                                  paramNameShort,
                        String                                  paramNameVerbose,
                        juce::AudioProcessorParameter::Category parameterCategory =
                            juce::AudioProcessorParameter::genericParameter);

    virtual ~GainMeterParameter() override = default;

    using Listener = MeterParameter::Listener;
};

}  // namespace bav
