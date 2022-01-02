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

namespace lemons::plugin::detail
{

[[nodiscard]] inline String paramNameToID (const String& name);

[[nodiscard]] inline std::function<String (float)> createDefaultValueToTextFunction (const String& paramLabel);

[[nodiscard]] inline std::function<float (const String&)> createDefaultTextToValueFunction();

template <typename ValueType>
[[nodiscard]] juce::NormalisableRange<float> createRange (ValueType minimum, ValueType maximum);


template <typename ValueType>
[[nodiscard]] std::function<String (ValueType, int)> createDefaultStringFromValueFunc (float rangeInterval);


template <typename ValueType>
[[nodiscard]] std::function<ValueType (const String&)> createDefaultValueFromStringFunc();


template <typename ValueType>
[[nodiscard]] std::function<String (float)> convertValToStringFunc (std::function<String (ValueType, int)> origFunc)
{
	if (origFunc == nullptr)
		origFunc = createDefaultStringFromValueFunc<ValueType> (1.f);

	return [=] (float value) -> String
	{ return origFunc (static_cast<ValueType> (value), 0); };
}


template <typename ValueType>
[[nodiscard]] std::function<float (const String&)> convertStringToValFunc (std::function<ValueType (const String&)> origFunc)
{
	if (origFunc == nullptr)
		origFunc = createDefaultValueFromStringFunc<ValueType>();

	return [=] (const String& text) -> float
	{ return static_cast<float> (origFunc (text)); };
}

}  // namespace lemons::plugin::detail
