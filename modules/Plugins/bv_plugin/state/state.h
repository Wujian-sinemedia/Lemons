
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

    ParameterList& getParameters();


    ToggleParam mainBypass {"Main bypass", false};

    Dimensions dimensions;

    StateToggler toggler {*this};

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

}  // namespace bav::plugin
