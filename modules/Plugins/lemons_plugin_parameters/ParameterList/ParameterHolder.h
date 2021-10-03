
#pragma once

namespace lemons::plugin
{
/** Base class for ParameterHolder that allows any kind of parameter to be accessed without knowing the full type.
    @see ParameterHolder, ParameterList
 */
struct ParamHolderBase : SerializableData
{
    /** Constructor.
        @param internal True if this parameter is "internal", IE, not automatable.
     */
    ParamHolderBase (bool internal = false);

    /** Destructor. */
    virtual ~ParamHolderBase() = default;

    /** Returns a pointer to the managed parameter object. */
    Parameter* operator->();

    /** The superclass ParameterHolder implements this to return a pointer to the managed parameter object. */
    virtual Parameter* getParam() const = 0;

    /** Adds the managed parameter object to the specified AudioProcessor. */
    void addTo (juce::AudioProcessor& processor);

    /** True if this parameter is "internal", IE, not automatable. */
    bool isInternal;

protected:
    void serialize (TreeReflector& ref) final;

    bool addedToProcessor {false};
};


//==============================================================================


/** Class that holds a parameter object, and can retain a reference to it after it has been added to an AudioProcessor.
    Example usage:
    @code
    juce::AudioProcessor myProcessor;
 
    using namespace lemons::plugin;
 
    ParameterHolder<ToggleParameter> myParam {"MyToggleParameter", true};
 
    myParam.addTo (myProcessor);
    @endcode
    Several typedefs are included in the lemons namespace to make it easier to forget that this wrapper class exists at all. For example:
    @code
    juce::AudioProcessor myProcessor;
 
    using namespace lemons;
 
    ToggleParam myParam {"MyToggleParameter", true};
 
    myParam.addTo (myProcessor);
    @endcode
    Implementing your own ParameterList makes this even easier to manage; see the docs for ParameterList to learn more.
    @tparam ParameterType The fully specialized type of parameter the holder will hold. This must inherit from my Parameter class.
    @see ParameterList, Parameter
 */
template < typename ParameterType >
class ParameterHolder : public ParamHolderBase
{
public:
    /** Constructs a new parameter holder and the parameter object it holds.
        All constructor arguments are forwarded verbatim to the managed parameter object.
     */
    template < typename... Args >
    explicit ParameterHolder (Args&&... args)
    {
        param = new ParameterType (std::forward< Args > (args)...);
    }

    /** Destructor.
        If the owned parameter object hasn't been added to an AudioProcessor, then this will destroy the parameter object.
     */
    ~ParameterHolder() override
    {
        if (! addedToProcessor)
            delete param;
    }

    /** Returns a pointer to the managed parameter object as a pointer to my Parameter base class. */
    Parameter* getParam() const final { return param; }

    /** Returns a pointer to the managed parameter object as its actual type. */
    ParameterType* get() const { return param; }

    /** Returns a reference to the managed parameter object. */
    operator ParameterType&() { return *param; }

    /** Returns a pointer to the managed parameter obejct. */
    ParameterType* operator->() { return param; }

    /** Returns a reference to the managed parameter object. */
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
