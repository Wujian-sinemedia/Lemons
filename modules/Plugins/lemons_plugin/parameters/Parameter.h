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
using juce::ValueTree;

/** @defgroup parameters
    @ingroup lemons_plugin
    Plugin parameter classes and utilities.
 */

/** @ingroup parameters
    A plugin parameter class.
    @see TypedParameter, ParameterTraits
 */
class Parameter : public juce::RangedAudioParameter
{
public:

	/** Creates a parameter.
	    @param paramName The name of this parameter.
	    @param paramRange The range of values that this parameter can have.
	    @param paramDefaultValue The default value of this parameter.
	    @param valueToTextFuncToUse An optional lambda function that converts this parameter's value to a text description.
	    @param textToValueFuncToUse An optional lambda function that converts a text input string to a value for this parameter.
	    @param paramLabel An optional label to use for this parameter's units.
	    @param isAutomatable Boolean flag that indicates whether this parameter should be automatable in the user's DAW.
	    @param metaParam Boolean flag that indicates whether this parameter represents a "meta-parameter" -- that is, a parameter that controls other parameters.
	    @param parameterCategory An optional parameter category. See juce::AudioProcessorParameter::Category.
	 */
	explicit Parameter (const String&         paramName,
	                    const ParameterRange& paramRange,
	                    float                 paramDefaultValue    = 1.f,
	                    BasicValToStringFunc  valueToTextFuncToUse = nullptr,
	                    BasicStringToValFunc  textToValueFuncToUse = nullptr,
	                    const String&         paramLabel           = {},
	                    bool                  isAutomatable        = true,
	                    bool                  metaParam            = false,
	                    ParameterCategory     parameterCategory    = ParameterCategory::genericParameter);

	/** Creates a parameter from a ParameterTraits object. */
	explicit Parameter (const ParameterTraits& traits);

	/** Returns the parameter's current value, in a normalized 0-1 range.
	    @see getDenormalizedValue()
	 */
	[[nodiscard]] float getNormalizedValue() const noexcept { return currentValue.load(); }

	/** Returns the parameter's current value, in the parameter's denormalized range.
	    @see getNormalizedValue()
	 */
	[[nodiscard]] float getDenormalizedValue() const noexcept { return denormalize (currentValue.load()); }

	/** Sets the parameter's value, in a normalized 0-1 range.
	    @see setDenormalizedValue()
	 */
	void setNormalizedValue (float value);

	/** Sets the parameter's value, in the parameter's denormalized range.
	    @see setNormalizedValue()
	 */
	void setDenormalizedValue (float value) { setNormalizedValue (normalize (value)); }

	/** Returns this parameter's range object. */
	[[nodiscard]] const ParameterRange& getNormalisableRange() const final { return range; }

	/** Returns true if this parameter is automatable by the user's DAW. */
	[[nodiscard]] bool isAutomatable() const final { return automatable; }

	/** Returns true if this parameter is a "meta parameter", ie, one that controls the values of other parameters. */
	[[nodiscard]] bool isMetaParameter() const final { return metaParameter; }


	//==============================================================================
    /** @name Functions for state saving and loading */
    ///@{

	/** Saves the state of this parameter to a ValueTree. */
	[[nodiscard]] virtual ValueTree saveToValueTree() const;

	/** Restores the parameter's state from a ValueTree. */
	virtual void loadFromValueTree (const ValueTree& tree);

	/** Returns a ParameterTraits object representing this parameter.
	    An object identical to this one can be recreated using the constructor that takes a ParameterTraits object.
	 */
	[[nodiscard]] virtual ParameterTraits getParameterTraits() const;

    ///@}

	//==============================================================================
    /** @name Functions for gesture state */
    ///@{

	/** Begins a change gesture for this parameter. */
	void beginGesture();

	/** Ends any active change gesture for this parameter. */
	void endGesture();

	/** Returns true if this parameter is currently in the middle of a change gesture. */
	[[nodiscard]] bool isChanging() const noexcept { return changing.load(); }

    ///@}

	//==============================================================================
    /** @name Functions for MIDI controller mapping */
    ///@{

	/** Returns the number of the MIDI controller mapped to this parameter, or -1 if this parameter is unmapped. */
	[[nodiscard]] int getMidiControllerNumber() const noexcept { return midiControllerNumber.load(); }

	/** Returns true if this parameter is currently mapped to a MIDI controller. */
	[[nodiscard]] bool isMidiControllerMapped() const noexcept { return midiControllerNumber.load() > -1; }

	/** Maps this parameter to listen for changes in a specified MIDI controller number. */
	void setMidiControllerNumber (int newControllerNumber);

	/** Removes the MIDI controller mapping, if any. */
	void removeMidiControllerMapping() noexcept { setMidiControllerNumber (-1); }

	/** Call this function with each MIDI CC message your plugin recieves, and the Parameter class will automatically update itself with changes in the appropriate controller, if a mapping is active.
	    @returns True if this parameter is mapped to the passed controller number.
	 */
	bool processNewControllerMessage (int controllerNumber, int controllerValue);

    ///@}

	//==============================================================================
    /** @name Functions related to the default value */
    ///@{

	/** Sets the parameter's current value as the default value. */
	void refreshDefault() { setNormalizedDefault (getNormalizedDefault()); }

	/** Resets the parameter to the saved default value. */
	void resetToDefault() { setNormalizedValue (getNormalizedDefault()); }

	/** Returns the parameter's default value, in a normalized 0-1 range.
	    @see getDenormalizedDefault()
	 */
	[[nodiscard]] float getNormalizedDefault() const noexcept { return currentDefault.load(); }

	/** Returns the parameter's default value, in the parameter's denormalized range.
	    @see getNormalizedDefault()
	 */
	[[nodiscard]] float getDenormalizedDefault() const noexcept { return denormalize (getNormalizedDefault()); }

	/** Sets the parameter's default, in a normalized 0-1 range.
	    @see setDenormalizedDefault()
	 */
	void setNormalizedDefault (float value);

	/** Sets the parameter's default, in the parameter's denormalized range.
	    @see setNormalizedDefault()
	 */
	void setDenormalizedDefault (float value) { setNormalizedDefault (normalize (value)); }

    ///@}

	//==============================================================================
	/** @name Functions for converting to/from text */
    ///@{

	/** Returns a textual description of a normalized value in the range 0-1.
	    @see getTextForDenormalizedValue()
	 */
	[[nodiscard]] String getTextForNormalizedValue (float value) const { return valueToTextFunc (value); }

	/** Returns a textual description of a value in the parameter's denormalized range.
	    @see getTextForNormalizedValue()
	 */
	[[nodiscard]] String getTextForDenormalizedValue (float value) const { return getTextForNormalizedValue (normalize (value)); }

	/** Returns a textual representation of the parameter's current value. */
	[[nodiscard]] String getTextForCurrentValue() const { return valueToTextFunc (currentValue.load()); }

	/** Returns a textual description of the parameter's maximum value.
	    @see getTextForMin()
	 */
	[[nodiscard]] String getTextForMax() const { return getTextForDenormalizedValue (range.end); }

	/** Returns a textual description of the parameter's minimum value.
	    @see getTextForMax()
	 */
	[[nodiscard]] String getTextForMin() const { return getTextForDenormalizedValue (range.start); }

    ///@}

	/*---------------------------------------------------------------------------------------------------------------------------*/

	/** A Listener class that will be notified when aspects of a Parameter change.
	 */
	struct Listener
	{
		/** Constructs a listener that listens to a specified Parameter.
		    Unlike the JUCE listeners' API, this class handles its own RAII with registering and deregistering itself for updates from the parameter.
		 */
		explicit Listener (Parameter& paramToUse);

		/** Destructor. */
		virtual ~Listener();

		/** Called when the parameter's value changes. */
		virtual void parameterValueChanged (float newNormalizedValue);

		/** Called when the parameter's gesture state changes. (IE, when gestures begin or end.) */
		virtual void gestureStateChanged (bool gestureIsStarting);

		/** Called when the default value of the parameter is changed. */
		virtual void parameterDefaultChanged (float newNormalizedDefault);

		/** Called when the MIDI controller that the parameter is mapped to changes.
		    Note that when the parameter's MIDI CC mapping is removed, this function will be called in the listeners with a value of -1 for the controller number.
		 */
		virtual void controllerNumberChanged (int newControllerNumber);

	private:
		Parameter& param;
	};


	//==============================================================================

	/** A listener class that fires lambdas for each callback.
	 */
	struct LambdaListener final : public Listener
	{
		explicit LambdaListener (Parameter& parameter,
		                         std::function<void (float)>
		                                                     valueChanged,
		                         std::function<void (float)> defaultChanged        = nullptr,
		                         std::function<void (bool)>  gestureChanged        = nullptr,
		                         std::function<void (int)>   midiControllerChanged = nullptr);

	private:
		void parameterValueChanged (float newNormalizedValue) final;
		void gestureStateChanged (bool gestureIsStarting) final;
		void parameterDefaultChanged (float newNormalizedDefault) final;
		void controllerNumberChanged (int newControllerNumber) final;

		std::function<void (float)> valueChangeFunc, defaultChangeFunc;
		std::function<void (bool)>  gestureChangeFunc;
		std::function<void (int)>   controllerChangeFunc;
	};

	/*---------------------------------------------------------------------------------------------------------------------------*/

	/** @internal */
	static constexpr auto id_prop = "id";


protected:

	/** Normalizes the passed value using the parameter's NormalisableRange object. */
	[[nodiscard]] float normalize (float input) const noexcept;

	/** Denormalizes the passed value using the parameter's NormalisableRange object. */
	[[nodiscard]] float denormalize (float input) const noexcept;

	/** @internal */
	static constexpr auto valueTreeType = "PARAM", value_prop = "value", default_prop = "default", controller_prop = "controllerNumber";

private:

	[[nodiscard]] float getValue() const final { return currentValue.load(); }

	void setValue (float newValue) final { currentValue.store (newValue); }

	[[nodiscard]] float getDefaultValue() const final { return getNormalizedDefault(); }

	[[nodiscard]] float getValueForText (const String& text) const final { return textToValueFunc (text); }


	//==============================================================================

	const bool automatable, metaParameter;

	const ParameterRange range;

	std::atomic<float> currentValue { 1.f }, currentDefault { 1.f };
	std::atomic<bool>  changing { false };
	std::atomic<int>   midiControllerNumber { -1 };

	BasicValToStringFunc valueToTextFunc;
	BasicStringToValFunc textToValueFunc;

	juce::ListenerList<Listener> listeners;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Parameter)
};

}  // namespace lemons::plugin
