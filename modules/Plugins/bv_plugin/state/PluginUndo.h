#pragma once

namespace bav::plugin
{
struct Undo : UndoManager
{
    Undo (StateBase& stateToManage)
        : UndoManager (stateToManage)
    {
        stateToManage.setUndoManager (*this);
    }
};

}  // namespace bav::plugin
