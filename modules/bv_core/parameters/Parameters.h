
#pragma once

namespace bav
{
class Parameter :   public bav::SerializableData,
                    private juce::AudioProcessorParameter::Listener
{
public:
    using RangedParam = juce::RangedAudioParameter;

    Parameter (RangedParam& p,
               float        defaultValue,
               juce::String paramNameShort,
               juce::String paramNameVerbose);

    virtual ~Parameter() override;

    bool operator== (const Parameter& other);

    void  refreshDefault();  // sets the parameter's current value to be the default value
    void  resetToDefault();  // resets the parameter's value to the default
    float getNormalizedDefault() const;
    float getDenormalizedDefault() const;
    void  setNormalizedDefault (float value);
    void  setDenormalizedDefault (float value);

    // these functions return the current denormalized value as the desired type literal
    float getFloatValue() const;
    int   getIntValue() const;
    bool  getBoolValue() const;

    float getCurrentNormalizedValue() const;
    float getCurrentDenormalizedValue() const;
    void  setNormalizedValue (float value);
    void  setDenormalizedValue (float value);

    float normalize (float input) const;
    float denormalize (float input) const;

    bool isChanging() const;

    void doAction();

    // if defined, these functions will be called on the message thread when this parameter changes ("somewhat synchronously"):
    std::function< void() > onParameterChange;
    std::function< void() > onDefaultChange;
    std::function< void (bool) >  onGestureStateChange;

    //==============================================================================

    RangedParam& rap;

    const juce::String parameterNameShort;
    const juce::String parameterNameVerbose;

    //==============================================================================

private:
    void         parameterValueChanged (int, float) override final;
    void         parameterGestureChanged (int, bool gestureIsStarting) override final;
    
    virtual void onGestureChange (bool gestureIsStarting) = 0;
    virtual void onAction() = 0;

    float currentDefault;
    bool  changing = false;

    float lastActionedValue;
};


//==============================================================================
//==============================================================================


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
        juce::String                            parameterLabel = juce::String(),
        juce::AudioProcessorParameter::Category parameterCategory =
            juce::AudioProcessorParameter::genericParameter,
        std::function< juce::String (float value, int maximumStringLength) >
                                                          stringFromValue = nullptr,
        std::function< float (const juce::String& text) > valueFromString = nullptr);

    float get() const;
    float getDefault() const;

    void set (float newValue);
    void setDefault (float newDefaultValue);

    void setAction (std::function< void (float) > action);

    const std::function< juce::String (float, int) >   floatToString;
    const std::function< float (const juce::String&) > stringToFloat;

    //==============================================================================

    struct Listener
    {
        virtual ~Listener() = default;

        void parameterValueChanged (float newValue);
        void parameterDefaultChanged (float newDefault);
        void parameterGestureStateChanged (bool paramIsChanging);
    };

    void addListener (Listener* l);
    void removeListener (Listener* l);

    //==============================================================================
    
private:
    void onGestureChange (bool gestureIsStarting) override final;
    void onAction() override final;
    
    void toValueTree (juce::ValueTree& tree) override final;
    void fromValueTree (const juce::ValueTree& tree) override final;
    
    std::function< void (float) > actionFunc;
    juce::ListenerList< Listener > listeners;
};


//==============================================================================
//==============================================================================


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
        juce::String parameterLabel = juce::String(),
        std::function< juce::String (int value, int maximumStringLength) >
                                                        stringFromInt = nullptr,
        std::function< int (const juce::String& text) > intFromString = nullptr);

    int get() const;
    int getDefault() const;

    void set (int newValue);
    void setDefault (int newDefaultValue);

    void setAction (std::function< void (int) > action);

    const std::function< juce::String (int, int) >   intToString;
    const std::function< int (const juce::String&) > stringToInt;

    //==============================================================================

    struct Listener
    {
        virtual ~Listener() = default;

        void parameterValueChanged (int newValue);
        void parameterDefaultChanged (int newDefault);
        void parameterGestureStateChanged (bool paramIsChanging);
    };

    void addListener (Listener* l);
    void removeListener (Listener* l);

    //==============================================================================

private:
    void onGestureChange (bool gestureIsStarting) override final;
    void onAction() override final;
    
    void toValueTree (juce::ValueTree& tree) override final;
    void fromValueTree (const juce::ValueTree& tree) override final;

    std::function< void (int) > actionFunc;
    juce::ListenerList< Listener > listeners;
};


//==============================================================================
//==============================================================================


class BoolParameter : public juce::AudioParameterBool, public bav::Parameter
{
public:
    using AudioParameterBool = juce::AudioParameterBool;

    BoolParameter (
        juce::String paramNameShort,
        juce::String paramNameVerbose,
        bool         defaultVal,
        juce::String parameterLabel = juce::String(),
        std::function< juce::String (bool value, int maximumStringLength) >
                                                         stringFromBool = nullptr,
        std::function< bool (const juce::String& text) > boolFromString = nullptr);

    bool get() const;
    bool getDefault() const;

    void set (bool newValue);
    void setDefault (bool newDefaultValue);

    void setAction (std::function< void (bool) > action);

    std::function< juce::String (bool, int) >        boolToString;
    std::function< bool (const juce::String& text) > stringToBool;

    //==============================================================================

    struct Listener
    {
        virtual ~Listener() = default;

        void parameterValueChanged (bool newValue);
        void parameterDefaultChanged (bool newDefault);
        void parameterGestureStateChanged (bool paramIsChanging);
    };

    void addListener (Listener* l);
    void removeListener (Listener* l);

    //==============================================================================

private:
    void onGestureChange (bool gestureIsStarting) override final;
    void onAction() override final;
    
    void toValueTree (juce::ValueTree& tree) override final;
    void fromValueTree (const juce::ValueTree& tree) override final;

    std::function< void (bool) > actionFunc;
    juce::ListenerList< Listener > listeners;
};


//==============================================================================
//==============================================================================


struct MeterParameter : public FloatParameter
{
    MeterParameter (
        juce::String paramNameShort,
        juce::String paramNameVerbose,
        juce::NormalisableRange< float >
                                                nRange,
        float                                   defaultVal,
        juce::String                            parameterLabel = juce::String(),
        juce::AudioProcessorParameter::Category parameterCategory =
            juce::AudioProcessorParameter::genericParameter,
        std::function< juce::String (float value, int maximumStringLength) >
                                                          stringFromValue = nullptr,
        std::function< float (const juce::String& text) > valueFromString = nullptr);

    bool isAutomatable() const override final { return false; }
};


//==============================================================================
//==============================================================================


struct GainMeterParameter : public MeterParameter
{
    GainMeterParameter (juce::String                            paramNameShort,
                        juce::String                            paramNameVerbose,
                        juce::AudioProcessorParameter::Category parameterCategory =
                            juce::AudioProcessorParameter::genericParameter);
};


}  // namespace bav
