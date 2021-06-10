#pragma once

namespace bav
{

class UndoManager
{
public:
    UndoManager (SerializableData& stateToManage);
    
    bool undo();
    bool redo();
    
    bool hasUndo() const;
    bool hasRedo() const;
    
    void undoToLastTransaction();
    
    void beginNewTransaction (const String& name);
    void endTransaction();
    
    juce::StringArray getUndoTransactionNames() const;
    juce::StringArray getRedoTransactionNames() const;
    
private:
    SerializableData& state;
    
    String transactionName;
    
    struct State
    {
        State (juce::ValueTree tree, const String& name);
        
        juce::ValueTree state;
        String transactionName;
    };
    
    std::vector<State> storedStates;
    
    int currentStep {0};
    
    void loadState (State& stateToLoad);
};

}
