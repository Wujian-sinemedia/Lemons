
#pragma once

namespace bav::plugin
{
struct Dimensions : SerializableData
{
    using SerializableData::SerializableData;

    Dimensions& operator= (const juce::Point< int >& newSize) noexcept;

    void serialize (TreeReflector& ref) final;

    juce::Point< int > get() const;

private:
    int width, height;
};


class StateBase : public SerializableData
{
public:
    StateBase (String pluginName, ParameterList& paramsToUse);

    void addTo (juce::AudioProcessor& processor);
    void addAllAsInternal();

    void setUndoManager (UndoManager& undo);

    ParameterList& getParameters();


    ToggleParam mainBypass {"Main bypass", false};

    Dimensions dimensions;

protected:
    void serialize (TreeReflector& ref) final;

    ParameterList& params;
};


template < typename ParamListType,
           BV_MUST_INHERIT_FROM (ParamListType, ParameterList) >
struct State : StateBase
{
    State (String pluginName)
        : StateBase (pluginName, parameters)
    {
    }

    ParamListType parameters;
};


class StatePackage : public SerializableData
{
public:
    StatePackage (StateBase& stateToUse)
        : state (stateToUse)
    {
        state.setUndoManager (undo);
        toggler.setUndoManager (undo);
    }

    StateBase& get() { return state; }

    StateToggler& getToggler() { return toggler; }

    UndoManager& getUndo() { return undo; }

private:
    void serialize (TreeReflector& ref)
    {
        ref.add ("State", state);
        ref.add ("Toggles", toggler);
    }

    StateBase& state;

    StateToggler toggler {state};

    UndoManager undo {*this};
};


template < typename StateType, BV_MUST_INHERIT_FROM (StateType, StateBase) >
struct PluginState : StatePackage
{
    PluginState()
        : StatePackage (state)
    {
    }

    StateType state;
};

}  // namespace bav::plugin
