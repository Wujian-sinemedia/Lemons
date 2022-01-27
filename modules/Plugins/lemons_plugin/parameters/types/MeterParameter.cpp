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

namespace lemons::plugin
{

template <typename ValueType>
MeterParameter<ValueType>::MeterParameter (ValueType min, ValueType max, ValueType defaultVal, const String& paramName,
										   ValToStringFunc<ValueType> stringFromValue,
										   StringToValFunc<ValueType> valueFromString,
										   const String&			  parameterLabel,
										   ParameterCategory		  parameterCategory)
	: TypedParameter<ValueType> (min, max, defaultVal, paramName, stringFromValue, valueFromString, parameterLabel, false, false, parameterCategory)
{
}

template <typename ValueType>
MeterParameter<ValueType>::MeterParameter (const ParameterTraits& traits)
	: MeterParameter (
		static_cast<ValueType> (traits.range.start), static_cast<ValueType> (traits.range.end), static_cast<ValueType> (traits.defaultValue), traits.name,
		detail::convertValToStringFuncToTyped<ValueType> (traits.valueToText, traits.label),
		detail::convertStringToValFuncToTyped<ValueType> (traits.textToValue),
		traits.label,
		traits.category)
{
}

template <typename ValueType>
ValueTree MeterParameter<ValueType>::saveToValueTree() const
{
	return {};
}

template <typename ValueType>
void MeterParameter<ValueType>::loadFromValueTree (const ValueTree&)
{
}

template struct MeterParameter<float>;
template struct MeterParameter<int>;
template struct MeterParameter<bool>;

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/


GainMeterParameter::GainMeterParameter (const String&	  paramName,
										ParameterCategory parameterCategory)

	: MeterParameter<float> (-60.f, 0.f, -60.f,
							 paramName,
							 gain_stringFromFloat,
							 gain_floatFromString,
							 TRANS ("dB"), parameterCategory)
{
}

}  // namespace lemons::plugin
