#pragma once

#include "PluginDimensions.h"
#include <lemons_plugin_parameters/lemons_plugin_parameters.h>

namespace lemons::plugin
{
class StateBase : public SerializableData
{
public:
    StateBase (String pluginName, ParameterList& paramsToUse, SerializableData* customStateDataToUse = nullptr);

    void addTo (juce::AudioProcessor& processor);
    void addAllAsInternal();

    ParameterList& getParameters();


    ToggleParam mainBypass {"Main bypass", false};

    Dimensions dimensions;

    ModulationManager modManager;

protected:
    void serialize (TreeReflector& ref) final;

    ParameterList& params;

    SerializableData* customStateData;
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


template < typename ParamListType, typename CustomDataType,
           BV_MUST_INHERIT_FROM (ParamListType, ParameterList),
           BV_MUST_INHERIT_FROM (CustomDataType, SerializableData) >
struct CustomState : StateBase
{
    CustomState (String pluginName)
        : StateBase (pluginName, parameters, &customData)
    {
    }

    ParamListType  parameters;
    CustomDataType customData;
};


}  // namespace bav::plugin
