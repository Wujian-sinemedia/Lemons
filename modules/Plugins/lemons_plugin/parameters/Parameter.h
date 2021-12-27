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

#include <juce_audio_processors/juce_audio_processors.h>

namespace lemons::plugin
{

using juce::String;
using juce::ValueTree;

/** A plugin parameter class.
    @see TypedParameter
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
	explicit Parameter (String paramName,
	                    juce::NormalisableRange<float>
	                                                            paramRange,
	                    float                                   paramDefaultValue    = 1.f,
	                    std::function<String (float)>           valueToTextFuncToUse = nullptr,
	                    std::function<float (const String&)>    textToValueFuncToUse = nullptr,
	                    String                                  paramLabel           = {},
	                    bool                                    isAutomatable        = true,
	                    bool                                    metaParam            = false,
	                    juce::AudioProcessorParameter::Category parameterCategory    = AudioProcessorParameter::genericParameter);

	/** Returns the parameter's current value, in a normalized 0-1 range.
	 @see getDenormalizedValue()
	 */
	[[nodiscard]] float getNormalizedValue() const noexcept;

	/** Returns the parameter's current value, in the parameter's denormalized range.
	 @see getNormalizedValue()
	 */
	[[nodiscard]] float getDenormalizedValue() const noexcept;

	/** Sets the parameter's value, in a normalized 0-1 range.
	 @see setDenormalizedValue()
	 */
	void setNormalizedValue (float value);

	/** Sets the parameter's value, in the parameter's denormalized range.
	 @see setNormalizedValue()
	 */
	void setDenormalizedValue (float value);

	/** Returns this parameter's range object. */
	[[nodiscard]] const juce::NormalisableRange<float>& getNormalisableRange() const final;

	/** Begins a change gesture for this parameter. */
	void beginGesture();

	/** Ends any active change gesture for this parameter. */
	void endGesture();

	/** Returns true if this parameter is currently in the middle of a change gesture. */
	[[nodiscard]] bool isChanging() const noexcept;


	// functions for MIDI controller mapping

	/** Returns the number of the MIDI controller mapped to this parameter, or -1 if this parameter is unmapped. */
	[[nodiscard]] int getMidiControllerNumber() const noexcept;

	/** Returns true if this parameter is currently mapped to a MIDI controller. */
	[[nodiscard]] bool isMidiControllerMapped() const noexcept;

	/** Maps this parameter to listen for changes in a specified MIDI controller number. */
	void setMidiControllerNumber (int newControllerNumber);

	/** Removes the MIDI controller mapping, if any. */
	void removeMidiControllerMapping() noexcept;

	/** Call this function with each MIDI CC message your plugin recieves, and the Parameter class will automatically update itself with changes in the appropriate controller, if a mapping is active.
        @returns True if this parameter is mapped to the passed controller number.
     */
	bool processNewControllerMessage (int controllerNumber, int controllerValue);


	// functions related to the default value

	/** Sets the parameter's current value as the default value. */
	void refreshDefault();

	/** Resets the parameter to the saved default value. */
	void resetToDefault();

	/** Returns the parameter's default value, in a normalized 0-1 range.
	    @see getDenormalizedDefault()
	 */
	[[nodiscard]] float getNormalizedDefault() const noexcept;

	/** Returns the parameter's default value, in the parameter's denormalized range.
	    @see getNormalizedDefault()
	 */
	[[nodiscard]] float getDenormalizedDefault() const noexcept;

	/** Sets the parameter's default, in a normalized 0-1 range.
	    @see setDenormalizedDefault()
	 */
	void setNormalizedDefault (float value);

	/** Sets the parameter's default, in the parameter's denormalized range.
	    @see setNormalizedDefault()
	 */
	void setDenormalizedDefault (float value);


	/** Sets an UndoManager to use to manage this parameter's transactions.
	    The UndoManager will track change gestures, changing the default value, and changing the MIDI CC mapping.
	    @see UndoManager
	 */
	//	void setUndoManager (UndoManager& managerToUse);

	/** Returns true if this parameter is automatable by the user's DAW. */
	[[nodiscard]] bool isAutomatable() const final;

	/** Returns a textual description of a normalized value in the range 0-1.
	    @see getTextForDenormalizedValue()
	 */
	[[nodiscard]] String getTextForNormalizedValue (float value) const;

	/** Returns a textual description of a value in the parameter's denormalized range.
	    @see getTextForNormalizedValue()
	 */
	[[nodiscard]] String getTextForDenormalizedValue (float value) const;

	/** Returns a textual description of the parameter's maximum value. */
	[[nodiscard]] String getTextForMax() const;

	/** Returns a textual description of the parameter's minimum value. */
	[[nodiscard]] String getTextForMin() const;

	/** Returns the name of the parameter.
	    @param maxLength The maximum length of the string to return, in characters. If this is less than 1, the max length will be unlimited.
	    @param internationalize If this is true, the returned string will be translated using the juce::LocalisedStrings class. Setting this to false is useful for serialization logic that relies on parameter names.
	 */
	[[nodiscard]] String getParameterName (int maxLength = 0, bool internationalize = true) const;
    
    
    [[nodiscard]] virtual ValueTree saveToValueTree() const;
    
    virtual void loadFromValueTree (const ValueTree& tree);

	//==============================================================================

	/** A Listener class that will be notified when aspects of a Parameter change.
	 */
	struct Listener
	{
		/** Constructs a listener that listens to a specified Parameter.
		    Unlike the JUCE listeners' API, this class handles its own RAII with registering and deregistering itself for updates from the parameter.
		 */
		Listener (Parameter& paramToUse);

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
    
    /** @internal */
    static constexpr auto id_prop = "id";

protected:

	[[nodiscard]] float normalize (float input) const noexcept;

	[[nodiscard]] float denormalize (float input) const noexcept;
    
    static constexpr auto valueTreeType = "PARAM", value_prop = "value", default_prop = "default", controller_prop = "controllerNumber";

private:
	bool isMetaParameter() const final;

	float getValue() const final;
	void  setValue (float newValue) final;
	float getDefaultValue() const final;

	void setValueInternal (float value);
	void setDefaultInternal (float value);

	float getValueForText (const String& text) const final;

	const bool automatable, metaParameter;

	const juce::NormalisableRange<float> range;

	std::atomic<float> currentValue { 1.f }, currentDefault { 1.f };
	std::atomic<bool>  changing { false };
	std::atomic<int>   midiControllerNumber { -1 };

	std::function<String (float)>        valueToTextFunc;
	std::function<float (const String&)> textToValueFunc;

	//	UndoManager* um = nullptr;

	juce::ListenerList<Listener> listeners;

	const String valueChangeTransactionName, defaultChangeTransactionName, midiControllerChangeTransactionName;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Parameter)
};

}  // namespace lemons::plugin
