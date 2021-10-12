#pragma once

namespace lemons
{
/**
    A class that keeps a history of states and can step backward and forward through them.
    This class is similar to juce's undo manager, but the major difference is that juce's implementation focuses on tracking individual actions, while this class simply saves a new copy of the entire state each time something changes. I have found this to be more robust, and easier to work with in other areas of the API. \n
    \n
    This object has a concept of grouping smaller actions into larger named transactions, similar to the API of the juce UndoManager.

    @see SerializableData
 */
class UndoManager
{
public:
    /** Creates an UndoManager.
        Must be given a serializable object to manage the state of. Don't delete the state object before you delete the undo manager! */
    UndoManager (SerializableData& stateToManage);

    /** Attempts to undo the last action.
        @return True if there was an action to undo.
     */
    bool undo();

    /** Attempts to redo the last action.
        @return True if there was an action to redo.
     */
    bool redo();

    /** Returns true if there is a cached action that can be undone. */
    bool hasUndo() const;

    /** Returns true if there is an action that has been undone and can be re-done. */
    bool hasRedo() const;

    /** Returns true if a change transaction is in progress. */
    bool isInMiddleOfTransaction() const;

    /** Undoes any actions taken since the last transaction finished. */
    void undoToLastTransaction();

    /** Begins a new aggregate transaction.
        @param name The name of this transaction. This can be empty, but it should preferrably be as descriptive as possible.
        @param force If this function is called while another transaction is still active (ie, before endTransaction() has been called), then the name of the current transaction will only be updated if the force flag is true.
     */
    void beginNewTransaction (const String& name = {}, bool force = false);

    /** Ends the current transaction, if there is one. */
    void endTransaction();

    /** Returns the name of the current transaction. */
    String getCurrentTransactionName() const;

    /** Returns the name of the cached transaction that's next in line to undo, if any. */
    String getNextUndoTransactionName() const;

    /** Returns the name of the cached transaction that's next in line to redo, if any. */
    String getNextRedoTransactionName() const;

    /** Returns the names of all cached transactions that can be undone. */
    juce::StringArray getUndoTransactionNames() const;

    /** Returns the names of all cached transactions that can be redone. */
    juce::StringArray getRedoTransactionNames() const;

    /** Clears the internal cache of stored states, and the entire undo/redo history. */
    void clearUndoHistory();

    /** Returns a Broadcaster object that fires every time a new state is loaded as a result of an undo or redo.
        @see events::Broadcaster
     */
    events::Broadcaster& getBroadcaster();

    /**
        An RAII class for beginning and ending a transaction in a scoped manner.
        Example usage:
        @code
        lemons::UndoManager undo;

        void someFunction()
        {
            lemons::UndoManager::ScopedTransaction s {&undo, "MyTransaction"};

            // do something with side-effects...
        }
        @endcode
     */
    struct ScopedTransaction
    {
        /** Creates a ScopedTransaction.
            @param um A pointer to the UndoManager that this ScopedTransaction should reference.
            This pointer is allowed to be null; I found myself writing several classes throughout the Lemons API that have functions to allow you to set the UndoManager after the creation of the class -- meaning that said class stores a pointer to an UndoManager, not a reference. So, instead of trying to conditionally create an RAII object, I put the null-pointer-checking logic inside the RAII object itself.
            @param name The name of the transaction that this object will create.
         */
        ScopedTransaction (UndoManager* um, const String& name = {});

        /** Destructor. */
        ~ScopedTransaction();

    private:
        UndoManager* undo;
    };

private:
    struct SavedState
    {
        SavedState (const ValueTree& tree, const String& name);

        ValueTree state;
        String    transactionName;
    };

    void saveState();
    void loadState (const SavedState& stateToLoad);

    SerializableData& state;

    String transactionName;
    bool   changing {false};

    std::vector< SavedState >            storedStates;
    std::vector< SavedState >::size_type currentStep {0};

    events::Broadcaster broadcaster;
};

}  // namespace lemons
