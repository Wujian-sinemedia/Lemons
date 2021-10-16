
/** @ingroup toggler
 *  @{
 */
#pragma once

namespace lemons::serializing
{
/** Interface that all Togglers implement to be able to access one without knowing its template arguments.
    @see Toggler
 */
struct TogglerInterface : SerializableData
{
    /** Loads the state at the specified index.
        This will deserialize the managed object from the state saved in the slot you've indicated. Indices start from 0.
        @return False if the passed index is out of range for this toggler.
     */
    virtual bool loadState (size_t num) = 0;

    /** Saves the current state of the managed object to the specified index.
        This will serialize the managed object and save the result in the slot you've indicated. Indices start from 0.
        @return False if the passed index is out of range for this toggler.
     */
    virtual bool saveState (size_t num) = 0;
};


/** Class that manages several versions of the state of a serializable object and allows you to switch between them.
    This is useful for A/B'ing plugin states, for example. \n \n
    For a GUI component that controls one of these, use the StateToggleButtons class. \n \n
    Note that this class is itself serializable; you can also save the state of your StateToggler with your plugin state. \n
    This class can also be used with an UndoManager.
    @see SerializableData, StateToggleButtons, UndoManager, TogglerInterface
 */
template < size_t numStates >
struct Toggler : TogglerInterface
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
        @return False if the passed index is out of range for this toggler.
     */
    bool loadState (size_t num) final
    {
        if (num >= numStates)
            return false;

        saveState (lastLoadedState);

        UndoManager::ScopedTransaction t {undo, "Loaded state " + String (num)};

        serializing::fromTree (states[num], state);
        lastLoadedState = num;
        return true;
    }

    /** Saves the current state of the managed object to the specified index.
        This will serialize the managed object and save the result in the slot you've indicated. Indices start from 0.
        @return False if the passed index is out of range for this toggler.
     */
    bool saveState (size_t num) final
    {
        if (num >= numStates)
            return false;

        states[num] = serializing::toTree (state);
        return true;
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
            loadState (lastLoadedState);
    }


    SerializableData& state;

    std::array< ValueTree, numStates > states;

    size_t lastLoadedState {0};

    UndoManager* undo {nullptr};
};

}  // namespace lemons::serializing

/** @}*/
