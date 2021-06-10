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

    bool isInMiddleOfTransaction() const { return changing; }

    void undoToLastTransaction();

    void beginNewTransaction (const String& name, bool force = false);
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
    void saveState();

    SerializableData& state;

    String transactionName;
    bool   changing {false};

    struct State
    {
        State (const juce::ValueTree& tree, const String& name);

        juce::ValueTree state;
        String          transactionName;
    };

    std::vector< State >            storedStates;
    std::vector< State >::size_type currentStep {0};

    void loadState (const State& stateToLoad);
};

}  // namespace bav
