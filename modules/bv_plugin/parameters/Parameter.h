
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

    virtual ~Parameter() override = default;

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

    void doAction (bool force = false);
    
    void sendListenerSyncCallback();  // sends a value update message immediately to all listeners

    //==============================================================================

    RangedParam& rap;

    const juce::String parameterNameShort;
    const juce::String parameterNameVerbose;

    //==============================================================================
    
    struct Listener
    {
        Listener (Parameter& paramToUse);
        virtual ~Listener();
        
        virtual void parameterValueChanged        (float newNormalizedValue);
        virtual void parameterGestureStateChanged (bool gestureIsStarting);
        virtual void parameterDefaultChanged      (float newNormalizedDefault);
        
    private:
        Parameter& param;
    };
    
    //==============================================================================

private:
    void setValueInternal (float newNormalizedValue);
    void setDefaultInternal (float newNormalizedDefault);
    
    virtual void onAction() { }

    float currentDefault;
    bool  changing = false;

    float lastActionedValue;
    
    juce::UndoManager* um;
    
    juce::ListenerList<Listener> listeners;
    
    const juce::String valueChangeTransactionName;
    const juce::String defaultChangeTransactionName;
    
    //==============================================================================
    
    class ValueChangeAction  :     public juce::UndoableAction
    {
    public:
        ValueChangeAction (Parameter& p, float newValue, float prevVal);
        
        bool perform() override final;
        bool undo() override final;
        
        UndoableAction* createCoalescedAction (UndoableAction* nextAction) override final;
        
    private:
        Parameter& param;
        const float targetValue;
        const float prevValue;
    };
    
    
    class DefaultChangeAction  :   public juce::UndoableAction
    {
    public:
        DefaultChangeAction (Parameter& p, float newNormalizedDefault, float prevNormDefault);
        
        bool perform() override final;
        bool undo() override final;
        
        UndoableAction* createCoalescedAction (UndoableAction* nextAction) override final;
        
    private:
        Parameter& param;
        const float targetDefault;
        const float prevDefault;
    };
};


}  // namespace bav
