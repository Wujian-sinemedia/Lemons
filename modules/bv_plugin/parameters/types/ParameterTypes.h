
#pragma once

namespace bav
{
class FloatParameter : public juce::AudioParameterFloat, public bav::Parameter
{
public:
    using AudioParameterFloat = juce::AudioParameterFloat;

    FloatParameter (
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
        String                                      parameterLabel  = String(),
        bool                                        isAutomatable   = true,
        bool                                        metaParam       = false);

    virtual ~FloatParameter() override = default;

    float get() const;
    float getDefault() const;

    void set (float newValue);
    void setDefault (float newDefaultValue);

    const std::function< String (float, int) >   floatToString;
    const std::function< float (const String&) > stringToFloat;

    bool isAutomatable() const final { return Parameter::automatable; }
    bool isMetaParameter() const final { return Parameter::metaParameter; }

    //==============================================================================

    struct Listener : public Parameter::Listener
    {
        Listener (FloatParameter& toUse);
        virtual ~Listener() override = default;

        virtual void paramValueChanged (float newValue);
        virtual void paramDefaultChanged (float newDefault);

    private:
        void parameterValueChanged (float newNormalizedValue) final;
        void parameterDefaultChanged (float newNormalizedDefault) final;

        FloatParameter& param;
    };
};


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/


class IntParameter : public juce::AudioParameterInt, public bav::Parameter
{
public:
    using AudioParameterInt = juce::AudioParameterInt;

    IntParameter (
        String paramNameShort,
        String paramNameVerbose,
        int    min,
        int    max,
        int    defaultVal,
        std::function< String (int value, int maximumStringLength) >
                                                  stringFromInt  = nullptr,
        std::function< int (const String& text) > intFromString  = nullptr,
        String                                    parameterLabel = juce::String(),
        bool                                      isAutomatable  = true,
        bool                                      metaParam      = false);

    virtual ~IntParameter() override = default;

    int get() const;
    int getDefault() const;

    void set (int newValue);
    void setDefault (int newDefaultValue);

    const std::function< String (int, int) >   intToString;
    const std::function< int (const String&) > stringToInt;

    bool isAutomatable() const final { return Parameter::automatable; }
    bool isMetaParameter() const final { return Parameter::metaParameter; }

    //==============================================================================

    struct Listener : public Parameter::Listener
    {
        Listener (IntParameter& toUse);
        virtual ~Listener() override = default;

        virtual void paramValueChanged (int newValue);
        virtual void paramDefaultChanged (int newDefault);

    private:
        void parameterValueChanged (float newNormalizedValue) override final;
        void parameterDefaultChanged (float newNormalizedDefault) override final;

        IntParameter& param;
    };
};


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/


class BoolParameter : public juce::AudioParameterBool, public bav::Parameter
{
public:
    using AudioParameterBool = juce::AudioParameterBool;

    BoolParameter (
        String paramNameShort,
        String paramNameVerbose,
        bool   defaultVal,
        std::function< String (bool value, int maximumStringLength) >
                                                   stringFromBool = nullptr,
        std::function< bool (const String& text) > boolFromString = nullptr,
        bool                                       isAutomatable  = true,
        bool                                       metaParam      = false);

    virtual ~BoolParameter() override = default;

    bool get() const;
    bool getDefault() const;

    void set (bool newValue);
    void setDefault (bool newDefaultValue);

    std::function< String (bool, int) >        boolToString;
    std::function< bool (const String& text) > stringToBool;

    bool isAutomatable() const final { return Parameter::automatable; }
    bool isMetaParameter() const final { return Parameter::metaParameter; }

    //==============================================================================

    struct Listener : public Parameter::Listener
    {
        Listener (BoolParameter& toUse);
        virtual ~Listener() override = default;

        virtual void paramValueChanged (bool newValue);
        virtual void paramDefaultChanged (bool newDefault);

    private:
        void parameterValueChanged (float newNormalizedValue) override final;
        void parameterDefaultChanged (float newNormalizedDefault) override final;

        BoolParameter& param;
    };
};

}  // namespace bav
