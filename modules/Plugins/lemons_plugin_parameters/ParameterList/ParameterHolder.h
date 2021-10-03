
#pragma once

namespace lemons::plugin
{
struct ParamHolderBase : SerializableData
{
    ParamHolderBase (bool internal = false);

    virtual ~ParamHolderBase() = default;

    Parameter* operator->();

    virtual Parameter* getParam() const = 0;

    void addTo (juce::AudioProcessor& processor);

    bool isInternal;

protected:
    void serialize (TreeReflector& ref) final;

    bool addedToProcessor {false};
};


//==============================================================================


template < typename ParameterType >
class ParameterHolder : public ParamHolderBase
{
public:
    template < typename... Args >
    explicit ParameterHolder (Args&&... args)
    {
        param = new ParameterType (std::forward< Args > (args)...);
    }

    ~ParameterHolder() override
    {
        if (! addedToProcessor)
            delete param;
    }

    Parameter*     getParam() const final { return param; }
    ParameterType* get() const { return param; }

                   operator ParameterType&() { return *param; }
    ParameterType* operator->() { return param; }
    ParameterType& operator*() { return *param; }

private:
    ParameterType* param;
};

}  // namespace lemons::plugin

namespace lemons
{
using FloatParam = plugin::ParameterHolder< plugin::FloatParameter >;
using IntParam   = plugin::ParameterHolder< plugin::IntParameter >;
using BoolParam  = plugin::ParameterHolder< plugin::BoolParameter >;

using dbParam        = plugin::ParameterHolder< plugin::GainParameter >;
using ToggleParam    = plugin::ParameterHolder< plugin::ToggleParameter >;
using PercentParam   = plugin::ParameterHolder< plugin::PercentParameter >;
using HzParam        = plugin::ParameterHolder< plugin::FrequencyParameter >;
using PitchParam     = plugin::ParameterHolder< plugin::MidiPitchParameter >;
using PanParam       = plugin::ParameterHolder< plugin::MidiPanParameter >;
using SemitonesParam = plugin::ParameterHolder< plugin::SemitonesParameter >;
using SecParam       = plugin::ParameterHolder< plugin::SecondsParameter >;

using GainMeter = plugin::ParameterHolder< plugin::GainMeterParameter >;

}  // namespace lemons
