
#pragma once

namespace bav
{
class FloatParameter : public juce::AudioParameterFloat, public bav::Parameter
{
public:
    using AudioParameterFloat = juce::AudioParameterFloat;

    FloatParameter (
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
        juce::String                                      parameterLabel  = juce::String(),
                    bool automatable = true,
                    bool metaParam = false);

    virtual ~FloatParameter() override = default;
    
    bool isAutomatable() const override final;
    bool isMetaParameter() const override final;

    float get() const;
    float getDefault() const;

    void set (float newValue);
    void setDefault (float newDefaultValue);

    void setAction (std::function< void (float) > action);

    const std::function< juce::String (float, int) >   floatToString;
    const std::function< float (const juce::String&) > stringToFloat;

    //==============================================================================

    struct Listener : public Parameter::Listener
    {
        Listener (FloatParameter& toUse);
        virtual ~Listener() override = default;

        virtual void paramValueChanged (float newValue);
        virtual void paramDefaultChanged (float newDefault);

    private:
        void parameterValueChanged (float newNormalizedValue) override final;
        void parameterDefaultChanged (float newNormalizedDefault) override final;

        FloatParameter& param;
    };

    //==============================================================================

private:
    void onAction() override final;

    void toValueTree (juce::ValueTree& tree) override final;
    void fromValueTree (const juce::ValueTree& tree) override final;

    std::function< void (float) > actionFunc = [] (float) {};
};


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/


class IntParameter : public juce::AudioParameterInt, public bav::Parameter
{
public:
    using AudioParameterInt = juce::AudioParameterInt;

    IntParameter (
        juce::String paramNameShort,
        juce::String paramNameVerbose,
        int          min,
        int          max,
        int          defaultVal,
        std::function< juce::String (int value, int maximumStringLength) >
                                                        stringFromInt  = nullptr,
        std::function< int (const juce::String& text) > intFromString  = nullptr,
        juce::String                                    parameterLabel = juce::String(),
                  bool automatable = true,
                  bool metaParam = false);

    virtual ~IntParameter() override = default;
    
    bool isAutomatable() const override final;
    bool isMetaParameter() const override final;

    int get() const;
    int getDefault() const;

    void set (int newValue);
    void setDefault (int newDefaultValue);

    void setAction (std::function< void (int) > action);

    const std::function< juce::String (int, int) >   intToString;
    const std::function< int (const juce::String&) > stringToInt;

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

    //==============================================================================

private:
    void onAction() override final;

    void toValueTree (juce::ValueTree& tree) override final;
    void fromValueTree (const juce::ValueTree& tree) override final;

    std::function< void (int) > actionFunc = [] (int) {};
};


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/


class BoolParameter : public juce::AudioParameterBool, public bav::Parameter
{
public:
    using AudioParameterBool = juce::AudioParameterBool;

    BoolParameter (
        juce::String paramNameShort,
        juce::String paramNameVerbose,
        bool         defaultVal,
        std::function< juce::String (bool value, int maximumStringLength) >
                                                         stringFromBool = nullptr,
        std::function< bool (const juce::String& text) > boolFromString = nullptr,
                   bool automatable = true,
                   bool metaParam = false);

    virtual ~BoolParameter() override = default;
    
    bool isAutomatable() const override final;
    bool isMetaParameter() const override final;

    bool get() const;
    bool getDefault() const;

    void set (bool newValue);
    void setDefault (bool newDefaultValue);

    void setAction (std::function< void (bool) > action);

    std::function< juce::String (bool, int) >        boolToString;
    std::function< bool (const juce::String& text) > stringToBool;

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

    //==============================================================================

private:
    void onAction() override final;

    void toValueTree (juce::ValueTree& tree) override final;
    void fromValueTree (const juce::ValueTree& tree) override final;

    std::function< void (bool) > actionFunc = [] (bool) {};
};


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

#if 0
class ChoiceParameter : public juce::AudioParameterChoice, public bav::Parameter
{
public:
    using AudioParameterChoice = juce::AudioParameterChoice;
    
    ChoiceParameter (
                  juce::String paramNameShort,
                  juce::String paramNameVerbose,
                  const juce::StringArray& choices,
                  int defaultItemIndex,
                  juce::String parameterLabel = juce::String());
    
    virtual ~ChoiceParameter() override = default;
    
    juce::String get() const;
    juce::String getDefault() const;
    
    void set (const juce::String& newValue);
    void setDefault (const juce::String& newDefaultValue);
    
    void setAction (std::function< void (const juce::String&) > action);
    
    //==============================================================================
    
    struct Listener : public Parameter::Listener
    {
        Listener (ChoiceParameter& toUse);
        virtual ~Listener() override = default;
        
        virtual void paramValueChanged (const juce::String& newValue);
        virtual void paramDefaultChanged (const juce::String& newDefault);
        
    private:
        void parameterValueChanged (float newNormalizedValue) override final;
        void parameterDefaultChanged (float newNormalizedDefault) override final;
        
        ChoiceParameter& param;
    };
    
    //==============================================================================
    
private:
    void onAction() override final;
    
    void toValueTree (juce::ValueTree& tree) override final;
    void fromValueTree (const juce::ValueTree& tree) override final;
    
    std::function< void (const juce::String&) > actionFunc = [](const juce::String&){ };
};
#endif

}  // namespace bav
