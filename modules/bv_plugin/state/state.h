
#pragma once

namespace bav
{

class StateBase :   public SerializableData
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
    
    void addTo (dsp::ProcessorBase& p);
    void addAllAsInternal();
    
    void refreshAllDefaults();
    void resetAllToDefault();
    
    void doAllActions (bool force = true);
    
    void sendCallbackToAllListeners();
    
    void setUndoManager (juce::UndoManager& um);
    
private:
    void toValueTree (ValueTree& tree) override final;
    void fromValueTree (const ValueTree& tree) override final;
    
    juce::Array<ParameterList*> lists;
    
    dsp::ProcessorBase* pb = nullptr;
};

}  // namespace
