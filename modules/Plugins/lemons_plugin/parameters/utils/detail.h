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
[[nodiscard]] inline juce::NormalisableRange<float> createRange (ValueType minimum, ValueType maximum);


template <typename ValueType>
[[nodiscard]] inline std::function<String (ValueType, int)> createDefaultStringFromValueFunc (float         rangeInterval,
                                                                                              const String& paramLabel = {});


template <typename ValueType>
[[nodiscard]] inline std::function<ValueType (const String&)> createDefaultValueFromStringFunc();


template <typename ValueType>
[[nodiscard]] inline std::function<String (float)> convertValToStringFuncFromTyped (std::function<String (ValueType, int)> origFunc,
                                                                                    const String&                          paramLabel = {});


template <typename ValueType>
[[nodiscard]] inline std::function<float (const String&)> convertStringToValFuncFromTyped (std::function<ValueType (const String&)> origFunc);


template <typename ValueType>
[[nodiscard]] inline std::function<String (ValueType, int)> convertValToStringFuncToTyped (std::function<String (float)> origFunc,
                                                                                           const String&                 paramLabel = {});


template <typename ValueType>
[[nodiscard]] inline std::function<ValueType (const String&)> convertStringToValFuncToTyped (std::function<float (const String&)> origFunc);

}  // namespace lemons::plugin::detail
