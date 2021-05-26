
#pragma once

namespace bav
{
class Parameter :   public bav::SerializableData,
                    private juce::AudioProcessorParameter::Listener
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

    float normalize (float input) const;
    float denormalize (float input) const;
    
    void setUndoManager (juce::UndoManager& managerToUse);

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
    
    juce::UndoManager* um;
};


}  // namespace bav
