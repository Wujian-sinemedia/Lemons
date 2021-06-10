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
    
    void clearUndoHistory();
    
    struct ScopedTransaction
    {
        ScopedTransaction (UndoManager* um, const String& name = {});
        ~ScopedTransaction();
        
    private:
        UndoManager* undo;
    };
    
private:
    SerializableData& state;
    
    String transactionName;
    
    struct State
    {
        State (const juce::ValueTree& tree, const String& name);
        
        juce::ValueTree state;
        String transactionName;
    };
    
    std::vector<State> storedStates;
    std::vector<State>::size_type currentStep {0};
    
    void loadState (const State& stateToLoad);
};

}
