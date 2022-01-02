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

using juce::ValueTree;

struct ParameterTraits final
{
	explicit ParameterTraits() = default;

	template <typename ValType>
	explicit ParameterTraits (ValType minimum, ValType maximum, ValType defaultVal,
	                          std::function<String (ValType, int)>   stringFromValue = nullptr,
	                          std::function<ValType (const String&)> valueFromString = nullptr);

	static constexpr auto valueTreeType = "ParameterTraits";

	[[nodiscard]] ValueTree toValueTree() const;

	[[nodiscard]] static ParameterTraits fromValueTree (const ValueTree& tree);

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

	using Category = juce::AudioProcessorParameter::Category;

	Category category { Category::genericParameter };
};


struct ParameterLayout final
{
	[[nodiscard]] static ParameterLayout fromValueTree (const ValueTree& tree);

	[[nodiscard]] ValueTree saveToValueTree() const;

	std::vector<ParameterTraits> parameters;

	static constexpr auto valueTreeType = "ParameterLayout";
};

}  // namespace lemons::plugin
