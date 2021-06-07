
#pragma once

namespace bav
{
class Parameter : public SerializableData
{
public:
    using RangedParam = juce::RangedAudioParameter;

    Parameter (RangedParam& p,
               String       paramNameShort,
               String       paramNameVerbose,
               bool         isAutomatable = true,
               bool         metaParam     = false);

    virtual ~Parameter() override = default;

    bool operator== (const Parameter& other);

    int  getMidiControllerNumber() const;
    bool isMidiControllerMapped() const;
    void setMidiControllerNumber (int newControllerNumber);
    void resetMidiControllerMapping();
    void processNewControllerMessage (int controllerNumber, int controllerValue);

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

    void sendListenerSyncCallback();  // sends a value update message immediately to all listeners

    //==============================================================================

    RangedParam& rap;

    const String parameterNameShort;
    const String parameterNameVerbose;

    const bool automatable;
    const bool metaParameter;

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

    //==============================================================================

private:
    void setValueInternal (float newNormalizedValue);
    void setDefaultInternal (float newNormalizedDefault);

    void toValueTree (juce::ValueTree& tree) final;
    void fromValueTree (const juce::ValueTree& tree) final;

    std::atomic< float > currentDefault;
    std::atomic< bool >  changing {false};
    std::atomic< int >   midiControllerNumber {-1};

    juce::UndoManager* um;

    juce::ListenerList< Listener > listeners;

    const String valueChangeTransactionName;
    const String defaultChangeTransactionName;

    //==============================================================================

    class ValueChangeAction : public juce::UndoableAction
    {
    public:
        ValueChangeAction (Parameter& p, float newValue, float prevVal);

        bool perform() final;
        bool undo() final;

        UndoableAction* createCoalescedAction (UndoableAction* nextAction) final;

    private:
        Parameter&  param;
        const float targetValue;
        const float prevValue;
    };


    class DefaultChangeAction : public juce::UndoableAction
    {
    public:
        DefaultChangeAction (Parameter& p, float newNormalizedDefault, float prevNormDefault);

        bool perform() final;
        bool undo() final;

        UndoableAction* createCoalescedAction (UndoableAction* nextAction) final;

    private:
        Parameter&  param;
        const float targetDefault;
        const float prevDefault;
    };
};


}  // namespace bav
