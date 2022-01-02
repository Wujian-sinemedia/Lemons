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

#include <juce_core/juce_core.h>


namespace lemons::plugin
{

using juce::String;

using BasicValToStringFunc = std::function<String (float)>;

template <typename ValueType>
using ValToStringFunc = std::function<String (ValueType, int)>;

using BasicStringToValFunc = std::function<float (const String&)>;

template <typename ValueType>
using StringToValFunc = std::function<ValueType (const String&)>;

}  // namespace lemons::plugin


namespace lemons::plugin::detail
{

[[nodiscard]] inline String paramNameToID (const String& name);

[[nodiscard]] inline BasicValToStringFunc createDefaultValueToTextFunction (const String& paramLabel);

[[nodiscard]] inline BasicStringToValFunc createDefaultTextToValueFunction();

template <typename ValueType>
[[nodiscard]] inline juce::NormalisableRange<float> createRange (ValueType minimum, ValueType maximum);


template <typename ValueType>
[[nodiscard]] inline ValToStringFunc<ValueType> createDefaultStringFromValueFunc (float         rangeInterval,
                                                                                  const String& paramLabel = {});


template <typename ValueType>
[[nodiscard]] inline StringToValFunc<ValueType> createDefaultValueFromStringFunc();


template <typename ValueType>
[[nodiscard]] inline BasicValToStringFunc convertValToStringFuncFromTyped (ValToStringFunc<ValueType> origFunc,
                                                                           const String&              paramLabel    = {},
                                                                           float                      rangeInterval = 1.f);


template <typename ValueType>
[[nodiscard]] inline BasicStringToValFunc convertStringToValFuncFromTyped (StringToValFunc<ValueType> origFunc);


template <typename ValueType>
[[nodiscard]] inline ValToStringFunc<ValueType> convertValToStringFuncToTyped (BasicValToStringFunc origFunc,
                                                                               const String&        paramLabel = {});


template <typename ValueType>
[[nodiscard]] inline StringToValFunc<ValueType> convertStringToValFuncToTyped (BasicStringToValFunc origFunc);

}  // namespace lemons::plugin::detail
