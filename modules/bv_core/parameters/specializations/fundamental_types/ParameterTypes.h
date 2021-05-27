
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
                    juce::String parameterLabel = juce::String());
    
    virtual ~FloatParameter() override = default;
    
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
        Listener (FloatParameter& toUse);
        virtual ~Listener();
        
        virtual void parameterValueChanged (float newValue);
        virtual void parameterDefaultChanged (float newDefault);
        virtual void parameterGestureStateChanged (bool paramIsChanging);
        
    private:
        FloatParameter& param;
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
                  stringFromInt = nullptr,
                  std::function< int (const juce::String& text) > intFromString = nullptr,
                  juce::String parameterLabel = juce::String());
    
    virtual ~IntParameter() override = default;
    
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
        Listener (IntParameter& toUse);
        virtual ~Listener();
        
        virtual void parameterValueChanged (int newValue);
        virtual void parameterDefaultChanged (int newDefault);
        virtual void parameterGestureStateChanged (bool paramIsChanging);
        
    private:
        IntParameter& param;
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
                   std::function< bool (const juce::String& text) > boolFromString = nullptr);
    
    virtual ~BoolParameter() override = default;
    
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
        Listener (BoolParameter& toUse);
        virtual ~Listener();
        
        virtual void parameterValueChanged (bool newValue);
        virtual void parameterDefaultChanged (bool newDefault);
        virtual void parameterGestureStateChanged (bool paramIsChanging);
        
    private:
        BoolParameter& param;
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


}  // namespace
