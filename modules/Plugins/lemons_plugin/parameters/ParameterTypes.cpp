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

#include "utils/TypedParameterHelpers.h"

namespace lemons::plugin
{

template <typename ValueType>
TypedParameter<ValueType>::TypedParameter (ValueType minimum,
                                           ValueType maximum,
                                           ValueType defaultValue,
                                           String    paramName,
                                           std::function<String (ValueType, int)>
                                               stringFromValue,
                                           std::function<ValueType (const String&)>
                                                                                   valueFromString,
                                           String                                  paramLabel,
                                           bool                                    isAutomatable,
                                           bool                                    metaParam,
                                           juce::AudioProcessorParameter::Category parameterCategory)
    : Parameter (
        paramName,
        createRange (minimum, maximum),
        static_cast<float> (defaultValue),
        convertValToStringFunc (stringFromValue),
        convertStringToValFunc (valueFromString),
        paramLabel, isAutomatable, metaParam, parameterCategory)
    , stringFromValueFunction (stringFromValue)
    , valueFromStringFunction (valueFromString)
{
	if (stringFromValueFunction == nullptr)
		stringFromValueFunction = createDefaultStringFromValueFunc<ValueType> (getNormalisableRange().interval);

	if (valueFromStringFunction == nullptr)
		valueFromStringFunction = createDefaultValueFromStringFunc<ValueType>();
}

template <>
[[nodiscard]] float TypedParameter<float>::get() const noexcept
{
	return this->getDenormalizedValue();
}

template <>
[[nodiscard]] int TypedParameter<int>::get() const noexcept
{
	return juce::roundToInt (this->getDenormalizedValue());
}

template <>
[[nodiscard]] bool TypedParameter<bool>::get() const noexcept
{
	return this->getNormalizedValue() >= 0.5f;
}

template <typename ValueType>
void TypedParameter<ValueType>::set (ValueType newValue)
{
	this->setDenormalizedValue (static_cast<float> (newValue));
}

template <>
[[nodiscard]] float TypedParameter<float>::getDefault() const noexcept
{
	return this->getDenormalizedDefault();
}

template <>
[[nodiscard]] int TypedParameter<int>::getDefault() const noexcept
{
	return juce::roundToInt (this->getDenormalizedDefault());
}

template <>
[[nodiscard]] bool TypedParameter<bool>::getDefault() const noexcept
{
	return this->getNormalizedDefault() >= 0.5f;
}

template <typename ValueType>
void TypedParameter<ValueType>::setDefault (ValueType newDefault)
{
	this->setDenormalizedDefault (static_cast<float> (newDefault));
}

template <typename ValueType>
[[nodiscard]] String TypedParameter<ValueType>::getStringForValue (ValueType value, int maxLength) const
{
	return stringFromValueFunction (value, maxLength);
}

template <typename ValueType>
[[nodiscard]] String TypedParameter<ValueType>::getStringForCurrentValue (int maxLength) const
{
	return stringFromValueFunction (get(), maxLength);
}

template <typename ValueType>
[[nodiscard]] ValueType TypedParameter<ValueType>::getValueForString (const String& string) const
{
	return valueFromStringFunction (string);
}

template <typename ValueType>
[[nodiscard]] ValueType TypedParameter<ValueType>::getMinimum() const noexcept
{
	return static_cast<ValueType> (this->getNormalisableRange().start);
}

template <typename ValueType>
[[nodiscard]] ValueType TypedParameter<ValueType>::getMaximum() const noexcept
{
	return static_cast<ValueType> (this->getNormalisableRange().end);
}

template <typename ValueType>
ValueTree TypedParameter<ValueType>::saveToValueTree() const
{
    ValueTree tree { valueTreeType };
    
    tree.setProperty (id_prop, getParameterID(), nullptr);
    
    tree.setProperty (value_prop, get(), nullptr);
    tree.setProperty (default_prop, getDefault(), nullptr);
    tree.setProperty (controller_prop, getMidiControllerNumber(), nullptr);
    
    return tree;
}

template <typename ValueType>
void TypedParameter<ValueType>::loadFromValueTree (const ValueTree& tree)
{
    if (! tree.hasType (valueTreeType))
        return;
    
    if (tree.hasProperty (value_prop))
        set ((ValueType) tree.getProperty (value_prop));
    
    if (tree.hasProperty (default_prop))
        setDefault ((ValueType) tree.getProperty (default_prop));
    
    if (tree.hasProperty (controller_prop))
        setMidiControllerNumber ((int) tree.getProperty (controller_prop));
}

template <typename ValueType>
TypedParameter<ValueType>::Listener::Listener (TypedParameter<ValueType>& param)
    : Parameter::Listener (param)
    , parameter (param)
{
}

template <typename ValueType>
void TypedParameter<ValueType>::Listener::parameterValueChanged (float)
{
	paramValueChanged (parameter.get());
}

template <typename ValueType>
void TypedParameter<ValueType>::Listener::parameterDefaultChanged (float)
{
	paramDefaultChanged (parameter.getDefault());
}


template class TypedParameter<float>;
template class TypedParameter<int>;
template class TypedParameter<bool>;


BoolParameter::BoolParameter (bool   defaultValue,
                              String paramName,
                              std::function<String (bool, int)>
                                  stringFromValue,
                              std::function<bool (const String&)>
                                                                      valueFromString,
                              String                                  paramLabel,
                              bool                                    isAutomatable,
                              bool                                    metaParam,
                              juce::AudioProcessorParameter::Category parameterCategory)
    : TypedParameter<bool> (false, true, defaultValue, paramName, stringFromValue, valueFromString, paramLabel, isAutomatable, metaParam, parameterCategory)
{
}

}  // namespace lemons::plugin
