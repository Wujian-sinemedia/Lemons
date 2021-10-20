
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
	ParamHolderBase (bool internal = false) noexcept;

	/** Destructor. */
	virtual ~ParamHolderBase() = default;

	/** Returns a pointer to the managed parameter object. */
	Parameter* operator->() const noexcept;

	/** The superclass ParameterHolder implements this to return a pointer to the managed parameter object. */
	[[nodiscard]] virtual Parameter* getParam() const noexcept = 0;

	/** Adds the managed parameter object to the specified AudioProcessor. */
	void addTo (juce::AudioProcessor& processor);

	/** True if this parameter is "internal", IE, not automatable. */
	bool isInternal;

protected:
	void serialize (TreeReflector& ref) final;

	bool addedToProcessor { false };
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
template <typename ParameterType>
class ParameterHolder : public ParamHolderBase
{
public:
	/** Constructs a new parameter holder and the parameter object it holds.
	    All constructor arguments are forwarded verbatim to the managed parameter object.
	 */
	template <typename... Args>
	explicit ParameterHolder (Args&&... args)
	{
		param = new ParameterType (std::forward<Args> (args)...);
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
	[[nodiscard]] Parameter* getParam() const noexcept final { return param; }

	/** Returns a pointer to the managed parameter object as its actual type. */
	[[nodiscard]] ParameterType* get() const noexcept { return param; }

	/** Returns a reference to the managed parameter object. */
	operator ParameterType&() noexcept { return *param; }

	/** Returns a pointer to the managed parameter obejct. */
	ParameterType* operator->() noexcept { return param; }

	/** Returns a reference to the managed parameter object. */
	ParameterType& operator*() noexcept { return *param; }

private:
	ParameterType* param;
};

}  // namespace lemons::plugin

namespace lemons
{
/** Any kind of generic float parameter.
    @see plugin::FloatParameter
 */
using FloatParam = plugin::ParameterHolder<plugin::FloatParameter>;

/** Any kind of generic integer parameter.
    @see plugin::IntParameter
 */
using IntParam = plugin::ParameterHolder<plugin::IntParameter>;

/** Any kind of generic boolean parameter.
    @see plugin::BoolParameter
 */
using BoolParam = plugin::ParameterHolder<plugin::BoolParameter>;

/** A parameter in decibels, used to represent gain, level, or gain reduction.
    @see plugin::GainParameter
 */
using dbParam = plugin::ParameterHolder<plugin::GainParameter>;

/** An on/off toggle parameter.
    @see plugin::ToggleParameter
 */
using ToggleParam = plugin::ParameterHolder<plugin::ToggleParameter>;

/** A 0-100 percent parameter.
    @see plugin::PercentParameter
 */
using PercentParam = plugin::ParameterHolder<plugin::PercentParameter>;

/** A frequency parameter in Hz.
    @see plugin::FrequencyParameter
 */
using HzParam = plugin::ParameterHolder<plugin::FrequencyParameter>;

/** A MIDI pitch parameter, in the range 0-127.
    @see plugin::MidiPitchParameter
 */
using PitchParam = plugin::ParameterHolder<plugin::MidiPitchParameter>;

/** A MIDI pan parameter, in the range 0-127.
    @see plugin::MidiPanParameter
 */
using PanParam = plugin::ParameterHolder<plugin::MidiPanParameter>;

/** A parameter for representing a number of semitones.
    @see plugin::SemitonesParameter
 */
using SemitonesParam = plugin::ParameterHolder<plugin::SemitonesParameter>;

/** A parameter for representing a number of seconds.
    @see plugin::SecondsParameter
 */
using SecParam = plugin::ParameterHolder<plugin::SecondsParameter>;

/** A parameter for outputting gain reduction from the plugin back to the DAW.
    @see plugin::GainMeterParameter
 */
using GainMeter = plugin::ParameterHolder<plugin::GainMeterParameter>;

}  // namespace lemons
