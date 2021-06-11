
#pragma once

namespace bav
{
class StateBase : public SerializableData
{
public:
    StateBase (juce::Identifier name);
    virtual ~StateBase() override = default;

    template < typename... Args >
    void add (ParameterList& list, Args&&... args)
    {
        add (list);
        add (std::forward< Args > (args)...);
    }
    void add (ParameterList& list);
    
    template < typename... Args >
    void addInternal (ParameterList& list, Args&&... args)
    {
        addInternal (list);
        addInternal (std::forward< Args > (args)...);
    }
    void addInternal (ParameterList& list);
    
    void addTo (dsp::ProcessorBase& p);
    void addTo (dsp::ProcessorBase* p);
    void addAllAsInternal();

    void refreshAllDefaults();
    void resetAllToDefault();

    void sendCallbackToAllListeners();

    void setUndoManager (UndoManager& um);

    void processMidi (const juce::MidiBuffer& midiMessages);
    void processMidiMessage (const juce::MidiMessage& message);
    
private:
    void addList (ParameterList& list, bool excludedFromPresets);
    
    void toValueTree (ValueTree& tree) final;
    void fromValueTree (const ValueTree& tree) final;

    juce::Array< ParameterList* > lists;
    dsp::ProcessorBase*           pb = nullptr;
    
    struct LastSavedEditorSize : SerializableData
    {
        LastSavedEditorSize (StateBase& b);
        void toValueTree (ValueTree& tree) final;
        void fromValueTree (const ValueTree& tree) final;

        StateBase& base;
    };

    LastSavedEditorSize lastSavedEditorSize {*this};
};

}  // namespace bav
