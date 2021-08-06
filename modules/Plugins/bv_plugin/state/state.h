#pragma once
#include "PluginDimensions.h"


namespace bav::plugin
{
class StateBase : public SerializableData
{
public:
    StateBase (String pluginName, ParameterList& paramsToUse);

    void addTo (juce::AudioProcessor& processor);
    void addAllAsInternal();

    ParameterList& getParameters();


    ToggleParam mainBypass {"Main bypass", false};

    Dimensions dimensions;

protected:
    void serialize (TreeReflector& ref) override;

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


struct StateSerializer : SerializableData
{
    StateSerializer (StateBase& stateToUse, StateToggler& togglerToUse);

private:
    void serialize (TreeReflector& ref) final;

    StateBase&    state;
    StateToggler& toggler;
};

}  // namespace bav::plugin
