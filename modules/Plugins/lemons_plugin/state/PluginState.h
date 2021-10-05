#pragma once

namespace lemons::plugin
{
/** Represents the top-level state of a plugin as a whole.
    This encapsulates your StateBase, State, or CustomState object along with a StateToggler and an UndoManager. \n
    If you use my Processor class, it will instantiate one of these objects with the state type you passed to the processor class's template.
    @tparam StateType The type of state object to use to represent the plugin's state. This type must inherit from StateBase.
    @see Processor, StateBase, StateToggler, UndoManager
 */
template < typename StateType, BV_MUST_INHERIT_FROM (StateType, StateBase) >
struct PluginState : SerializableData
{
    /** Creates a new plugin state. */
    PluginState()
    {
        state.getParameters().setUndoManager (undo);
        toggles.setUndoManager (undo);
    }

    /** The plugin's main state object. */
    StateType state;

    /** The plugin's A/B/C state toggler. By default, it has 3 state slots. */
    StateToggles toggles {state};

    /** The plugin's undo manager. */
    UndoManager undo {state};

private:
    void serialize (TreeReflector& ref) final
    {
        ref.add ("State", state);
        ref.add ("Toggles", toggles);
    }
};

}  // namespace lemons::plugin
