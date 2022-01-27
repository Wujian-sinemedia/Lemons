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

/** @ingroup fundamental_parameter_types
	A parameter that can be stored as a type other than a float, but still inherits from my Parameter base class.
	@tparam ValueType The type as which the parameter's value is stored. Can be float, int, or bool.
	@see Parameter
 */
template <typename ValueType>
class TypedParameter : public Parameter
{
public:

	/** Creates a typed parameter.
		This constructor allows you to specify the minimum and maximum values, instead of supplying a range object.
		@param minimum The minimum value the parameter can have.
		@param maximum The maximum value the parameter can have.
		@param defaultValue The initial default value for the parameter.
		@param paramName The name of this parameter.
		@param stringFromValue An optional lambda function that converts this parameter's value to a text description.
		@param valueFromString An optional lambda function that converts a text input string to a value for this parameter.
		@param paramLabel An optional label to use for this parameter's units.
		@param isAutomatable Boolean flag that indicates whether this parameter should be automatable in the user's DAW.
		@param metaParam Boolean flag that indicates whether this parameter represents a "meta-parameter" -- that is, a parameter that controls other parameters.
		@param parameterCategory An optional parameter category. See juce::AudioProcessorParameter::Category.
	*/
	explicit TypedParameter (ValueType					minimum,
							 ValueType					maximum,
							 ValueType					defaultValue,
							 const String&				paramName,
							 ValToStringFunc<ValueType> stringFromValue	  = nullptr,
							 StringToValFunc<ValueType> valueFromString	  = nullptr,
							 const String&				paramLabel		  = {},
							 bool						isAutomatable	  = true,
							 bool						metaParam		  = false,
							 ParameterCategory			parameterCategory = ParameterCategory::genericParameter);

	/** Creates a typed parameter from a ParameterTraits object. */
	explicit TypedParameter (const ParameterTraits& traits);


	/** Returns the parameter's current value. */
	[[nodiscard]] ValueType get() const noexcept;

	/** Sets the parameter's value. */
	void set (ValueType newValue);

	/** Returns the minimum possible value for this parameter. */
	[[nodiscard]] ValueType getMinimum() const noexcept;

	/** Returns the maximum possible value for this parameter. */
	[[nodiscard]] ValueType getMaximum() const noexcept;

	/** Returns the parameter's current default value. */
	[[nodiscard]] ValueType getDefault() const noexcept;

	/** Sets the parameter's default value. */
	void setDefault (ValueType newDefault);

	/** Returns a textual description for a parameter value. */
	[[nodiscard]] String getStringForValue (ValueType value, int maxLength = 50) const;

	/** Returns a textual description of the parameter's current value. */
	[[nodiscard]] String getStringForCurrentValue (int maxLength = 50) const;

	/** Converts some user input text to a possible representation as a parameter value. */
	[[nodiscard]] ValueType getValueForString (const String& string) const;

	/** Saves the state of this parameter to a ValueTree. */
	[[nodiscard]] virtual ValueTree saveToValueTree() const override;

	/** Restores the parameter's state from a ValueTree. */
	virtual void loadFromValueTree (const ValueTree& tree) override;

	/** Returns a ParameterTraits object representing this parameter. */
	[[nodiscard]] ParameterTraits getParameterTraits() const final;

	/** The parameter's value type is publicly accessible through this typedef. */
	using ParameterValueType = ValueType;

	/*---------------------------------------------------------------------------------------------------------------------------*/

	/** A class that will receive updates when aspects of a TypedParameter change.
	 */
	struct Listener : public Parameter::Listener
	{
		/** Constructs a listener that listens to a specified parameter.
			Unlike the JUCE listeners' API, this class handles its own RAII with registering and deregistering itself for updates from the parameter.
		*/
		explicit Listener (TypedParameter<ValueType>& param);

		/** Called when the parameter's value is changed. */
		virtual void paramValueChanged (ValueType) { }

		/** Called when the parameter's default value is changed. */
		virtual void paramDefaultChanged (ValueType) { }

	private:

		void parameterValueChanged (float) final;
		void parameterDefaultChanged (float) final;

		TypedParameter<ValueType>& parameter;
	};

	//==============================================================================

	/** A listener that fires lambdas for each callback. */
	struct LambdaListener final : public Listener
	{
	public:

		explicit LambdaListener (TypedParameter<ValueType>& parameter,
								 std::function<void (ValueType)>
																 valueChanged,
								 std::function<void (ValueType)> defaultChanged	   = nullptr,
								 std::function<void (bool)>		 gestureChanged	   = nullptr,
								 std::function<void (int)>		 controllerChanged = nullptr);

	private:

		void paramValueChanged (ValueType newValue) final;
		void paramDefaultChanged (ValueType newDefault) final;
		void gestureStateChanged (bool gestureIsStarting) final;
		void controllerNumberChanged (int newControllerNumber) final;

		std::function<void (ValueType)> valueChangeFunc, defaultChangeFunc;
		std::function<void (bool)>		gestureChangeFunc;
		std::function<void (int)>		controllerChangeFunc;
	};

	/*---------------------------------------------------------------------------------------------------------------------------*/

private:

	ValToStringFunc<ValueType> stringFromValueFunction;
	StringToValFunc<ValueType> valueFromStringFunction;
};


/** @ingroup fundamental_parameter_types
	Represents a parameter with a float value. */
using FloatParameter = TypedParameter<float>;

/** @ingroup fundamental_parameter_types
	Represents a parameter with an integer value. */
using IntParameter = TypedParameter<int>;


/** @ingroup fundamental_parameter_types
	Helper struct that simplifies the constructor for boolean parameters.
	Bool parameters could technically be a TypedParameter<bool>, but this struct simplifies the constructor a bit.
 */
struct BoolParameter : TypedParameter<bool>
{
	/** Creates a boolean parameter.
		@param defaultValue Sets whether the parameter should be on or off by default.
		@param paramName The name of this parameter.
		@param stringFromValue An optional lambda function that converts this parameter's value to a text description.
		@param valueFromString An optional lambda function that converts a text input string to a value for this parameter.
		@param paramLabel An optional label to use for this parameter's units.
		@param isAutomatable Boolean flag that indicates whether this parameter should be automatable in the user's DAW.
		@param metaParam Boolean flag that indicates whether this parameter represents a "meta-parameter" -- that is, a parameter that controls other parameters.
		@param parameterCategory An optional parameter category. See juce::AudioProcessorParameter::Category.
	*/
	explicit BoolParameter (bool				  defaultValue,
							const String&		  paramName,
							ValToStringFunc<bool> stringFromValue	= nullptr,
							StringToValFunc<bool> valueFromString	= nullptr,
							const String&		  paramLabel		= {},
							bool				  isAutomatable		= true,
							bool				  metaParam			= false,
							Category			  parameterCategory = Category::genericParameter);

	/** Creates a boolean parameter from a ParameterTraits object. */
	explicit BoolParameter (const ParameterTraits& traits);
};


}  // namespace lemons::plugin
