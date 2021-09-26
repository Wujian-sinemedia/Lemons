#pragma once

#include <lemons_serializing/lemons_serializing.h>

namespace bav::plugin
{
struct StateToggler : SerializableData
{
    StateToggler (SerializableData& stateToUse);

    void loadStateA();
    void saveStateA();

    void loadStateB();
    void saveStateB();

    void loadStateC();
    void saveStateC();

    void loadLastSelectedState();
    void saveLastSelectedState();

    void setUndoManager (UndoManager& undoManager);

private:
    void serialize (TreeReflector& ref) final;

    SerializableData& state;

    ValueTree stateA {"StateA"}, stateB {"StateB"}, stateC {"StateC"};

    int lastLoadedState {1};

    UndoManager* undo {nullptr};
};

}  // namespace bav::plugin
