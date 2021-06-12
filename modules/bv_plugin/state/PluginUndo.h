#pragma once

namespace bav
{

struct PluginUndo : UndoManager
{
    PluginUndo (ParameterList& listToManage)
    : UndoManager(listToManage)
    {
        listToManage.setUndoManager (*this);
    }
    
    PluginUndo (StateBase& stateToManage)
    : UndoManager(stateToManage)
    {
        stateToManage.setUndoManager (*this);
    }
};

}
