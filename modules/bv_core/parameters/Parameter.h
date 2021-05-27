
#pragma once

namespace bav
{
class Parameter :   public bav::SerializableData
{
public:
    using RangedParam = juce::RangedAudioParameter;

    Parameter (RangedParam& p,
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

    float getCurrentNormalizedValue() const;
    float getCurrentDenormalizedValue() const;
    void  setNormalizedValue (float value);
    void  setDenormalizedValue (float value);
    
    void beginGesture();
    void endGesture();
    bool isChanging() const;

    float normalize (float input) const;
    float denormalize (float input) const;
    
    void setUndoManager (juce::UndoManager& managerToUse);

    void doAction();

    //==============================================================================

    RangedParam& rap;

    const juce::String parameterNameShort;
    const juce::String parameterNameVerbose;

    //==============================================================================
    
    struct Listener
    {
        Listener (Parameter& paramToUse);
        virtual ~Listener();
        
        virtual void parameterValueChanged (float newNormalizedValue);
        virtual void parameterGestureStateChanged (bool gestureIsStarting);
        virtual void parameterDefaultChanged (float newNormalizedDefault);
        
    private:
        Parameter& param;
    };
    
    void addParameterListener (Listener* l);
    void removeParameterListener (Listener* l);
    
    //==============================================================================

private:
    virtual void onAction() { }

    float currentDefault;
    bool  changing = false;

    float lastActionedValue;
    
    juce::UndoManager* um;
    
    juce::ListenerList<Listener> listeners;
    
    const juce::String valueChangeTransactionName;
    const juce::String defaultChangeTransactionName;
};


}  // namespace bav
