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

#include <juce_data_structures/juce_data_structures.h>
#include <juce_audio_processors/juce_audio_processors.h>


namespace lemons::plugin
{

class Parameter;

class ParameterList;

using juce::String;
using juce::ValueTree;
using ParameterCategory = juce::AudioProcessorParameter::Category;


struct ParameterTraits final
{
	explicit ParameterTraits() = default;

	template <typename ValType>
	explicit ParameterTraits (ValType minimum, ValType maximum, ValType defaultVal,
	                          const String& nameToUse, const String& labelToUse,
	                          std::function<String (ValType, int)>   stringFromValue = nullptr,
	                          std::function<ValType (const String&)> valueFromString = nullptr,
	                          bool automatable = true, bool metaParameter = false,
	                          ParameterCategory categoryToUse = ParameterCategory::genericParameter);

	static constexpr auto valueTreeType = "ParameterTraits";

	[[nodiscard]] ValueTree toValueTree() const;

	[[nodiscard]] static ParameterTraits fromValueTree (const ValueTree& tree);

	[[nodiscard]] bool isValid() const;

	[[nodiscard]] std::unique_ptr<Parameter> createParameter (const ParameterList& list) const;

	enum class ValueType
	{
		floatValue,
		intValue,
		boolValue
	};

	ValueType valueType { ValueType::floatValue };

	String name;
	String label;

	juce::NormalisableRange<float> range;
	float                          defaultValue { 1.f };

	std::function<String (float)>        valueToText { detail::createDefaultValueToTextFunction (label) };
	std::function<float (const String&)> textToValue { detail::createDefaultTextToValueFunction() };

	bool isAutomatable { true };
	bool isMetaParameter { false };

	ParameterCategory category { ParameterCategory::genericParameter };
};


struct ParameterLayout final
{
	explicit ParameterLayout() = default;

	[[nodiscard]] static ParameterLayout fromValueTree (const ValueTree& tree);

	[[nodiscard]] ValueTree saveToValueTree() const;

	std::vector<ParameterTraits> parameters;

	static constexpr auto valueTreeType = "ParameterLayout";
};

}  // namespace lemons::plugin
