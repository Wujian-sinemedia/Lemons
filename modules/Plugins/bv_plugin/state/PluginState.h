#pragma once

namespace bav::plugin
{
template < typename StateType, BV_MUST_INHERIT_FROM (StateType, StateBase) >
struct PluginState : SerializableData
{
    PluginState()
    {
        state.getParameters().setUndoManager (undo);
        toggles.setUndoManager (undo);
    }

    void serialize (TreeReflector& ref) final
    {
        ref.add ("State", state);
        ref.add ("Toggles", toggles);
    }

    StateType    state;
    StateToggler toggles {state};
    UndoManager  undo {state};
};

}  // namespace bav::plugin
