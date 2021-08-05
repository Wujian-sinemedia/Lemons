
namespace bav::plugin
{
StateToggler::StateToggler (SerializableData& stateToUse)
    : state (stateToUse)
{
}

void StateToggler::loadLastSelectedState()
{
    switch (lastLoadedState)
    {
        case 1 :
            loadStateA();
            break;

        case 2 :
            loadStateB();
            break;

        case 3 :
            loadStateC();
            break;

        default :
            lastLoadedState = 1;
            break;
    }
}

void StateToggler::saveLastSelectedState()
{
    switch (lastLoadedState)
    {
        case 1 :
            saveStateA();
            break;

        case 2 :
            saveStateB();
            break;

        case 3 :
            saveStateC();
            break;

        default :
            lastLoadedState = 1;
            break;
    }
}

void StateToggler::loadStateA()
{
    UndoManager::ScopedTransaction t {undo};

    serializing::fromTree (stateA, state);
    lastLoadedState = 1;
}

void StateToggler::saveStateA()
{
    stateA = serializing::toTree (state);
}

void StateToggler::loadStateB()
{
    UndoManager::ScopedTransaction t {undo};

    serializing::fromTree (stateB, state);
    lastLoadedState = 2;
}

void StateToggler::saveStateB()
{
    stateB = serializing::toTree (state);
}

void StateToggler::loadStateC()
{
    UndoManager::ScopedTransaction t {undo};

    serializing::fromTree (stateC, state);
    lastLoadedState = 3;
}

void StateToggler::saveStateC()
{
    stateC = serializing::toTree (state);
}

void StateToggler::serialize (TreeReflector& ref)
{
    ref.add ("StateA", stateA);
    ref.add ("StateB", stateB);
    ref.add ("StateC", stateC);
    ref.add ("LastLoadedState", lastLoadedState);
}

void StateToggler::setUndoManager (UndoManager& undoManager)
{
    undo = &undoManager;
}

}  // namespace bav::plugin
