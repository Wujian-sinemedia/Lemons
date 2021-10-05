#pragma once

namespace lemons::serializing
{
/** Class that manages several versions of the state of a serializable object and allows you to switch between them.
    This is useful for A/B'ing plugin states, for example. \n \n
    For a GUI component that controls one of these, use the StateToggleButtons class. \n \n
    Note that this class is itself serializable; you can also save the state of your StateToggler with your plugin state. \n
    This class can also be used with an UndoManager.
    @see SerializableData, StateToggleButtons, UndoManager
 */
template < size_t numStates >
struct Toggler : SerializableData
{
    /** Creates a StateToggler.
        @param stateToUse The serializable object to manage the state of.
     */
    Toggler (SerializableData& stateToUse)
        : state (stateToUse)
    {
    }

    /** Loads the state at the specified index.
        This will deserialize the managed object from the state saved in the slot you've indicated. Indices start from 0.
     */
    void loadState (size_t num)
    {
        saveLastSelectedState();
        UndoManager::ScopedTransaction t {undo, "Loaded state " + String (num)};
        serializing::fromTree (states[num], state);
        lastLoadedState = num;
    }

    /** Saves the current state of the managed object to the specified index.
        This will serialize the managed object and save the result in the slot you've indicated. Indices start from 0.
     */
    void saveState (size_t num)
    {
        states[num] = serializing::toTree (state);
    }

    /** Sets the UndoManager to use.
        If an UndoManager is assigned, then calling any of the loadState functions will create an undoable transaction.
     */
    void setUndoManager (UndoManager& undoManager)
    {
        undo = &undoManager;
    }

private:
    void serialize (TreeReflector& ref) final
    {
        ref.add ("State", states);
        ref.add ("LastLoadedState", lastLoadedState);

        if (ref.isLoading())
            loadLastSelectedState();
    }

    void loadLastSelectedState()
    {
        loadState (lastLoadedState);
    }

    void saveLastSelectedState()
    {
        saveState (lastLoadedState);
    }

    SerializableData& state;

    std::array< ValueTree, numStates > states;

    size_t lastLoadedState {0};

    UndoManager* undo {nullptr};
};

}  // namespace lemons::serializing
