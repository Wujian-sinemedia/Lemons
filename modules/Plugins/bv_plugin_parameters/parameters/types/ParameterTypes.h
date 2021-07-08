
#pragma once

namespace bav::plugin
{
class FloatParameter : public juce::AudioParameterFloat, public Parameter
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

    float get() const;
    float getDefault() const;

    void set (float newValue);
    void setDefault (float newDefaultValue);
    
    float getMinimum() const;
    float getMaximum() const;

    const std::function< String (float, int) >   floatToString;
    const std::function< float (const String&) > stringToFloat;

    bool isAutomatable() const final { return Parameter::automatable; }
    bool isMetaParameter() const final { return Parameter::metaParameter; }

    //==============================================================================

    struct Listener : public Parameter::Listener
    {
        Listener (FloatParameter& toUse);
        
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


class IntParameter : public juce::AudioParameterInt, public Parameter
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

    int get() const;
    int getDefault() const;

    void set (int newValue);
    void setDefault (int newDefaultValue);
    
    int getMinimum() const;
    int getMaximum() const;

    const std::function< String (int, int) >   intToString;
    const std::function< int (const String&) > stringToInt;

    bool isAutomatable() const final { return Parameter::automatable; }
    bool isMetaParameter() const final { return Parameter::metaParameter; }

    //==============================================================================

    struct Listener : public Parameter::Listener
    {
        Listener (IntParameter& toUse);
        
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


class BoolParameter : public juce::AudioParameterBool, public Parameter
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
       
        virtual void paramValueChanged (bool newValue);
        virtual void paramDefaultChanged (bool newDefault);

    private:
        void parameterValueChanged (float newNormalizedValue) override final;
        void parameterDefaultChanged (float newNormalizedDefault) override final;

        BoolParameter& param;
    };
};

}  // namespace bav
