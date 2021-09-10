
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
            return;

        case 2 :
            loadStateB();
            return;

        case 3 :
            loadStateC();
            return;

        default :
            lastLoadedState = 1;
            return;
    }
}

void StateToggler::saveLastSelectedState()
{
    switch (lastLoadedState)
    {
        case 1 :
            saveStateA();
            return;

        case 2 :
            saveStateB();
            return;

        case 3 :
            saveStateC();
            return;

        default :
            lastLoadedState = 1;
            return;
    }
}

#define bvst_implement_load_state_func(valueTreeName, number) \
    UndoManager::ScopedTransaction t {undo};                  \
    serializing::fromTree (valueTreeName, state);             \
    lastLoadedState = number

void StateToggler::loadStateA()
{
    bvst_implement_load_state_func (stateA, 1);
}

void StateToggler::loadStateB()
{
    bvst_implement_load_state_func (stateB, 2);
}

void StateToggler::loadStateC()
{
    bvst_implement_load_state_func (stateC, 3);
}

#undef bvst_implement_load_state_func


void StateToggler::saveStateA()
{
    stateA = serializing::toTree (state);
}

void StateToggler::saveStateB()
{
    stateB = serializing::toTree (state);
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

    if (ref.isLoading())
        loadLastSelectedState();
}

void StateToggler::setUndoManager (UndoManager& undoManager)
{
    undo = &undoManager;
}

}  // namespace bav::plugin
