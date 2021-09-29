
#pragma once

#include "ParameterHolder.h"

namespace lemons::plugin
{
class ParameterList : public SerializableData
{
public:
    ParameterList (juce::Identifier name, UndoManager* um = nullptr);

    void addParametersTo (juce::AudioProcessor& processor);
    void addAllParametersAsInternal();

    void refreshAllDefaults();
    void resetAllToDefault();

    void setUndoManager (UndoManager& um);

    void processMidi (const MidiBuffer& midiMessages);
    void processMidiMessage (const MidiMessage& message);

    BV_DECLARE_RECURSIVE_VARIADIC_FUNCTION (add, ParamHolderBase&)
    BV_DECLARE_RECURSIVE_VARIADIC_FUNCTION (addInternal, ParamHolderBase&)

    void setPitchbendParameter (IntParam& param);
    void setLastMovedMidiControllerNumberParameter (IntParam& param);
    void setLastMovedMidiControllerValueParameter (IntParam& param);

    /* recieves a callback when any of the list's parameters change */
    struct Listener
    {
        Listener (ParameterList& list,
                  std::function< void (Parameter&) >
                                                           onParamChange,
                  std::function< void (Parameter&, bool) > onGestureGhange       = {},
                  bool                                     includeInternalParams = true);

        virtual ~Listener() = default;

    private:
        OwnedArray< ParamUpdater > updaters;
    };

private:
    void serialize (TreeReflector& ref) final;

    void addParameter (ParamHolderBase& param, bool isInternal);

    void processNewControllerMessage (int controllerNumber, int controllerValue);
    void processNewPitchwheelMessage (int pitchwheelValue);

    std::vector< ParamHolderBase* > params;

    dsp::BasicProcessorBase dummyProcessor;

    IntParameter* pitchwheelParameter {nullptr};
    IntParameter* lastMovedControllerNumberParameter {nullptr};
    IntParameter* lastMovedControllerValueParameter {nullptr};

    UndoManager* undo {nullptr};
};


}  // namespace bav::plugin
