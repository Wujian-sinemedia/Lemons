#pragma once

namespace bav::plugin
{
struct StateToggler : SerializableData
{
    StateToggler (StateBase& stateToUse);

    void loadStateA();
    void saveStateA();

    void loadStateB();
    void saveStateB();

    void loadStateC();
    void saveStateC();

    void loadLastSelectedState();
    void saveLastSelectedState();

private:
    void serialize (TreeReflector& ref) final;

    StateBase& state;

    ValueTree stateA {"StateA"}, stateB {"StateB"}, stateC {"StateC"};

    int lastLoadedState {1};
};

}  // namespace bav::plugin
