/*
 ======================================================================================

 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.

 ======================================================================================
 */

#pragma once


namespace lemons::plugin
{

using juce::String;


/** @defgroup plugin_hosting Plugin hosting
    @ingroup lemons_plugin
    Plugin hosting classes.
 */


/** @ingroup plugin_hosting
    This class holds a reference to a juce::AudioProcessor, and acts as a wrapper around it, providing some convenience and utility functions.
    To instantiate a custom subclass of AudioProcessor and pass it into one of these automatically, use the TypedProcessorHolder class.
    @see TypedProcessorHolder
 */
struct ProcessorHolder
{
	/** Creates a ProcessorHolder that manages the specified AudioProcessor. */
	explicit ProcessorHolder (juce::AudioProcessor& processorToUse);

	/** Destructor. */
	virtual ~ProcessorHolder() = default;

	/** Returns a pointer to the managed AudioProcessor. */
	juce::AudioProcessor* operator->();

	/** Returns a pointer to the managed AudioProcessor. */
	const juce::AudioProcessor* operator->() const;

	/** Prepares the processor for playback.
	    Internally, this sets the appropriate processing precision and buses layout and calls prepareToPlay().
	 */
	template <typename SampleType>
	bool prepareForPlayback (double samplerate, int blocksize, int numChannels = 2);

    
    /** @name Accessing parameters */
    ///@{
    
	/** Finds a parameter with a specified name. */
	[[nodiscard]] juce::AudioProcessorParameter* getNamedParameter (const String& name);

	/** Finds a parameter with a specified name. */
	[[nodiscard]] const juce::AudioProcessorParameter* getNamedParameter (const String& name) const;

	/** Finds a parameter with a specified name, and attempts to cast it to the specified subclass of juce::AudioProcessorParameter.
	    @tparam ParameterType The type of parameter class you want to access the parameter as. This type must inherit from juce::AudioProcessorParameter.
	 */
	template <typename ParameterType, LEMONS_MUST_INHERIT_FROM (ParameterType, juce::AudioProcessorParameter)>
	[[nodiscard]] ParameterType* getTypedParameter (const String& name)
	{
		if (auto* p = getNamedParameter (name))
			return dynamic_cast<ParameterType*> (p);

		return nullptr;
	}

	/** Finds a parameter with a specified name, and attempts to cast it to the specified subclass of juce::AudioProcessorParameter.
	    @tparam ParameterType The type of parameter class you want to access the parameter as. This type must inherit from juce::AudioProcessorParameter.
	 */
	template <typename ParameterType, LEMONS_MUST_INHERIT_FROM (ParameterType, juce::AudioProcessorParameter)>
	[[nodiscard]] const ParameterType* getTypedParameter (const String& name) const
	{
		if (const auto* p = getNamedParameter (name))
			return dynamic_cast<const ParameterType*> (p);

		return nullptr;
	}
    
    ///@}
    

	/** A simple representation of a parameter's state. */
	struct ParameterStateData
	{
		String name;
		float  value;
	};

	/** A simple representation of a processor's state. */
	using ParameterState = juce::Array<ParameterStateData>;

	/** Returns the current state of the processor's parameters. */
	[[nodiscard]] ParameterState getStateOfParameters() const;

	/** Restores a previous parameter state. */
	void setStateOfParameters (const ParameterState& state);

	/** The managed audio processor. */
	juce::AudioProcessor& processor;
};


/** @ingroup plugin_hosting
    A ProcessorHolder that instantiates a custom subclass of AudioProcessor and automatically creates a ProcessorHolder for you.
    @tparam ProcessorType The type of processor you wish to instantiate. This type must inherit from juce::AudioProcessor.
    @see ProcessorHolder
 */
template <typename ProcessorType, LEMONS_MUST_INHERIT_FROM (ProcessorType, juce::AudioProcessor)>
struct TypedProcessorHolder final : public ProcessorHolder
{
	/** Constructor. */
	TypedProcessorHolder()
	    : ProcessorHolder (typedProcessor)
	{
	}

	/** The managed processor. */
	ProcessorType typedProcessor;
};

}  // namespace lemons::plugin
