
#pragma once

namespace bav::plugin
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
    
    void setUndoManager (UndoManager& managerToUse);

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
        virtual void parameterControllerNumberChanged (int newControllerNumber);

    private:
        Parameter& param;
    };

    //==============================================================================

private:
    void serialize (TreeReflector& ref) final;

    void setValueInternal (float value);
    void setDefaultInternal (float value);
    void setMidiControllerInternal (int controller);

    std::atomic< float > currentDefault;
    std::atomic< bool >  changing {false};
    std::atomic< int >   midiControllerNumber {-1};

    UndoManager* um = nullptr;

    juce::ListenerList< Listener > listeners;

    const String valueChangeTransactionName;
    const String defaultChangeTransactionName;
    const String midiControllerChangeTransactionName;
};

}  // namespace bav
