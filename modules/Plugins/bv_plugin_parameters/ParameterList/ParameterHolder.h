
#pragma once

namespace bav
{
struct ParamHolderBase
{
    ParamHolderBase (bool internal = false) : isInternal (internal) { }
    
    Parameter* operator->() { return getParam(); }

    virtual ~ParamHolderBase() = default;

    virtual Parameter* getParam() const = 0;

    void addTo (juce::AudioProcessor& processor);

    bool isInternal;

protected:
    bool addedToProcessor = false;
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
        if (! ParamHolderBase::addedToProcessor)
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


using FloatParam = ParameterHolder< FloatParameter >;
using IntParam   = ParameterHolder< IntParameter >;
using BoolParam  = ParameterHolder< BoolParameter >;

using dbParam      = ParameterHolder< GainParameter >;
using ToggleParam  = ParameterHolder< ToggleParameter >;
using PercentParam = ParameterHolder< PercentParameter >;
using HzParam      = ParameterHolder< FrequencyParameter >;

using GainMeter = bav::ParameterHolder< bav::GainMeterParameter >;

}  // namespace bav
