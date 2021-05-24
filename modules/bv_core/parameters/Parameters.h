
#pragma once

namespace bav
{
class Parameter : private juce::AudioProcessorParameter::Listener
{
public:
    using RangedParam = juce::RangedAudioParameter;

    Parameter (int          keyID,
               RangedParam& p,
               float        defaultValue,
               juce::String paramNameShort,
               juce::String paramNameVerbose);

    virtual ~Parameter() override;

    bool operator== (const Parameter& other) const { return key == other.key; }

    bool operator= (float value) { rap.setValueNotifyingHost (value); }

    //==============================================================================

    float getNormalizedDefault() const;
    float getDenormalizedDefault() const;
    void  setNormalizedDefault (float value);

    // these functions set the denormalized default
    void setDefault (float value);
    void setDefault (int value);
    void setDefault (bool value);

    // sets the parameter's current value to be the default value
    void refreshDefault();

    // resets the parameter's value to the default
    void resetToDefault();

    //==============================================================================

    float getCurrentNormalizedValue() const;
    float getCurrentDenormalizedValue() const;

    // these functions return the current denormalized value as the desired type literal
    float getFloatValue() const;
    int   getIntValue() const;
    bool  getBoolValue() const;

    //==============================================================================

    // returns a float value normalized in the range 0 to 1, using this parameter's NormalisableRange object
    float normalize (float input) const;
    float normalize (int input) const;
    float normalize (bool input) const;

    // takes a normalized value as input and returns a denormalized float value within the natural range of this parameter.
    float denormalize (float input) const;

    //==============================================================================
    /* You can specify an action of a certain type, to be performed synchronously when you call doAction(), only if the parameter calue has changed since the last call to doAction().
           You can only specify one type of action at a time. Each type will be called with the denormalized current parameter value in its natural range -- so, float actions will recieve denormalized values, int actions will recieve denormalized floats rounded to integers, and bool actions will recieve boolean literals. You can also specify a void action, that will be called with no arguments.
         */

    void doAction();

    void setFloatAction (std::function< void (float) > action);
    void setIntAction (std::function< void (int) > action);
    void setBoolAction (std::function< void (bool) > action);
    void setVoidAction (std::function< void() > action);

    // if defined, these functions will be called on the message thread when this parameter changes ("somewhat synchronously")
    std::function< void (float) >
                                  onParameterChange;  // this gets the denormalized new float value
    std::function< void (float) > onDefaultChange;
    std::function< void (bool) >  onGestureStateChange;

    bool isChanging() const;

    //==============================================================================

    virtual juce::ValueTree toValueTree() const = 0;

    virtual void restoreFromValueTree (const juce::ValueTree& tree) = 0;

    //==============================================================================

    RangedParam& rap;

    const int key;

    const juce::String parameterNameShort;
    const juce::String parameterNameVerbose;

    //==============================================================================

private:
    void parameterValueChanged (int, float) override final;
    void parameterGestureChanged (int, bool gestureIsStarting) override final;

    std::atomic< float > currentDefault;
    std::atomic< bool >  changing;

    std::function< void (float) > floatAction;
    std::function< void (int) >   intAction;
    std::function< void (bool) >  boolAction;
    std::function< void() >       voidAction;

    float lastActionedValue;
};


//==============================================================================


class FloatParameter : public juce::AudioParameterFloat, public bav::Parameter
{
public:
    using AudioParameterFloat = juce::AudioParameterFloat;

    FloatParameter (
        int          keyID,
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

    float getDefault() const;

    std::function< juce::String (float, int) >   floatToString;
    std::function< float (const juce::String&) > stringToFloat;

    juce::ValueTree toValueTree() const override;

    void restoreFromValueTree (const juce::ValueTree& tree) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FloatParameter)
};


//==============================================================================


class IntParameter : public juce::AudioParameterInt, public bav::Parameter
{
public:
    using AudioParameterInt = juce::AudioParameterInt;

    IntParameter (
        int          keyID,
        juce::String paramNameShort,
        juce::String paramNameVerbose,
        int          min,
        int          max,
        int          defaultVal,
        juce::String parameterLabel = juce::String(),
        std::function< juce::String (int value, int maximumStringLength) >
                                                        stringFromInt = nullptr,
        std::function< int (const juce::String& text) > intFromString = nullptr);

    int getDefault() const;

    std::function< juce::String (int, int) >   intToString;
    std::function< int (const juce::String&) > stringToInt;

    juce::ValueTree toValueTree() const override;

    void restoreFromValueTree (const juce::ValueTree& tree) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IntParameter)
};


//==============================================================================


class BoolParameter : public juce::AudioParameterBool, public bav::Parameter
{
public:
    using AudioParameterBool = juce::AudioParameterBool;

    BoolParameter (
        int          keyID,
        juce::String paramNameShort,
        juce::String paramNameVerbose,
        bool         defaultVal,
        juce::String parameterLabel = juce::String(),
        std::function< juce::String (bool value, int maximumStringLength) >
                                                         stringFromBool = nullptr,
        std::function< bool (const juce::String& text) > boolFromString = nullptr);

    bool getDefault() const;

    std::function< juce::String (bool, int) >        boolToString;
    std::function< bool (const juce::String& text) > stringToBool;

    juce::ValueTree toValueTree() const override;

    void restoreFromValueTree (const juce::ValueTree& tree) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BoolParameter)
};


//==============================================================================


struct MeterParameter : public FloatParameter
{
    MeterParameter (
        int          keyID,
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

    juce::ValueTree toValueTree() const override;

    void restoreFromValueTree (const juce::ValueTree& tree) override;

    bool isAutomatable() const override final { return false; }
};


//==============================================================================


struct GainMeterParameter : public MeterParameter
{
    GainMeterParameter (int                                     keyID,
                        juce::String                            paramNameShort,
                        juce::String                            paramNameVerbose,
                        juce::AudioProcessorParameter::Category parameterCategory =
                            juce::AudioProcessorParameter::genericParameter);
};


}  // namespace bav
