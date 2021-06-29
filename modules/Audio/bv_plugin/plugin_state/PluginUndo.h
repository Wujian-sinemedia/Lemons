#pragma once

namespace bav
{
struct PluginUndo : UndoManager
{
    PluginUndo (ParameterList& listToManage)
        : UndoManager (listToManage)
    {
        listToManage.setUndoManager (*this);
    }

    PluginUndo (PluginState& stateToManage)
        : UndoManager (stateToManage)
    {
        stateToManage.getParameters().setUndoManager (*this);
    }
};

}  // namespace bav
