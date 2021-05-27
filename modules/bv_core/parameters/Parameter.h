
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

    // if defined, these functions will be called on the message thread when this parameter changes ("somewhat synchronously"):
    std::function< void() > onParameterChange;
    std::function< void() > onDefaultChange;
    std::function< void (bool) >  onGestureStateChange;

    //==============================================================================

    RangedParam& rap;

    const juce::String parameterNameShort;
    const juce::String parameterNameVerbose;

    //==============================================================================
    
    struct Listener
    {
        Listener (Parameter& paramToUse);
        virtual ~Listener();
        
        virtual void valueChanged (float newNormalizedValue);
        virtual void gestureStateChanged (bool gestureIsStarting);
        virtual void defaultChanged (float newNormalizedDefault);
        
    private:
        Parameter& param;
    };
    
    void addListener (Listener* l) { listeners.add (l); }
    void removeListener (Listener* l) { listeners.add (l); }
    
    //==============================================================================

private:
    virtual void onAction() = 0;

    float currentDefault;
    bool  changing = false;

    float lastActionedValue;
    
    juce::UndoManager* um;
    
    juce::ListenerList<Listener> listeners;
};


}  // namespace bav
