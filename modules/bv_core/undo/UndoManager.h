#pragma once

namespace bav
{
class UndoManager
{
public:
    UndoManager (SerializableData& stateToManage);
    virtual ~UndoManager() = default;

    bool undo();
    bool redo();

    bool hasUndo() const;
    bool hasRedo() const;

    bool isInMiddleOfTransaction() const { return changing; }

    void undoToLastTransaction();

    void beginNewTransaction (const String& name = {}, bool force = false);
    void endTransaction();

    String            getCurrentTransactionName() const { return transactionName; }
    String            getNextUndoTransactionName() const;
    String            getNextRedoTransactionName() const;
    juce::StringArray getUndoTransactionNames() const;
    juce::StringArray getRedoTransactionNames() const;

    void clearUndoHistory();

    events::Broadcaster& getBroadcaster() { return broadcaster; }

    struct ScopedTransaction
    {
        ScopedTransaction (UndoManager* um, const String& name = {});
        ~ScopedTransaction();

    private:
        UndoManager* undo;
    };

private:
    struct State
    {
        State (const juce::ValueTree& tree, const String& name);

        juce::ValueTree state;
        String          transactionName;
    };

    void saveState();
    void loadState (const State& stateToLoad);

    SerializableData& state;

    String transactionName;
    bool   changing {false};

    std::vector< State >            storedStates;
    std::vector< State >::size_type currentStep {0};

    events::Broadcaster broadcaster;
};

}  // namespace bav
