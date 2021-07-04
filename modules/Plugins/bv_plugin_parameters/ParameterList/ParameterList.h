
#pragma once

namespace bav
{
class ParameterList : public SerializableData
{
public:
    ParameterList (juce::Identifier name, UndoManager* um = nullptr);

    void addParametersTo (juce::AudioProcessor& processor);
    void addAllParametersAsInternal();

    void refreshAllDefaults();
    void resetAllToDefault();

    void sendCallbackToAllListeners();
    
    void setUndoManager (UndoManager& um);

    void processMidi (const MidiBuffer& midiMessages);
    void processMidiMessage (const MidiMessage& message);

    /* recieves a callback when any of the list's parameters change */
    struct Listener
    {
        Listener (ParameterList& list,
                  std::function< void (Parameter&) >
                                                           onParamChange,
                  std::function< void (Parameter&, bool) > onGestureGhange       = {},
                  bool                                     includeInternalParams = true);

    private:
        juce::OwnedArray< ParamUpdater > updaters;
    };

    BV_DECLARE_RECURSIVE_VARIADIC_FUNCTION (add, ParamHolderBase&)
    BV_DECLARE_RECURSIVE_VARIADIC_FUNCTION (add, StringProperty&)

    BV_DECLARE_RECURSIVE_VARIADIC_FUNCTION (addInternal, StringProperty&)
    BV_DECLARE_RECURSIVE_VARIADIC_FUNCTION (addInternal, ParamHolderBase&)

    void setPitchbendParameter (IntParam& param);
    void setLastMovedMidiControllerNumberParameter (IntParam& param);
    void setLastMovedMidiControllerValueParameter (IntParam& param);

private:
    virtual void toValueTree (ValueTree&) final { }
    virtual void fromValueTree (const ValueTree&) final { }

    void addParameter (ParamHolderBase& param, bool isInternal);
    void addStringProperty (StringProperty& property);

    void processNewControllerMessage (int controllerNumber, int controllerValue);
    void processNewPitchwheelMessage (int pitchwheelValue);

    juce::Array< ParamHolderBase* > params;
    dsp::BasicProcessorBase         dummyProcessor;

    juce::Array< StringProperty* > strings;

    IntParameter* pitchwheelParameter;
    IntParameter* lastMovedControllerNumberParameter;
    IntParameter* lastMovedControllerValueParameter;
    
    UndoManager* undo;
};


}  // namespace bav
