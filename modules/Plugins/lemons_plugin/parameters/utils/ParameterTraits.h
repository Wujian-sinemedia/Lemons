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
#include <lemons_core/lemons_core.h>

namespace lemons::plugin
{

class Parameter;

class ParameterList;

using juce::ValueTree;


/**
 */
struct ParameterTraits final
{
	explicit ParameterTraits() = default;

	template <typename ValType>
	explicit ParameterTraits (ValType minimum, ValType maximum, ValType defaultVal,
	                          const String& nameToUse, const String& labelToUse,
	                          ValToStringFunc<ValType> stringFromValue = nullptr,
	                          StringToValFunc<ValType> valueFromString = nullptr,
	                          bool automatable = true, bool metaParameter = false,
	                          ParameterCategory categoryToUse = ParameterCategory::genericParameter);

	[[nodiscard]] ValueTree toValueTree() const;

	[[nodiscard]] static ParameterTraits fromValueTree (const ValueTree& tree);

	/** @internal */
	static constexpr auto valueTreeType = "ParameterTraits";

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

	ParameterRange range;
	float          defaultValue { 1.f };

	BasicValToStringFunc valueToText { detail::createDefaultValueToTextFunction (label) };
	BasicStringToValFunc textToValue { detail::createDefaultTextToValueFunction() };

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

	/** @internal */
	static constexpr auto valueTreeType = "ParameterLayout";
};

}  // namespace lemons::plugin


namespace lemons::binary
{

template <files::FileType Type = files::FileType::JSON>
[[nodiscard]] plugin::ParameterLayout getParameterLayout (const String& fileName);

}


namespace lemons::files
{

template <FileType Type = FileType::JSON>
[[nodiscard]] plugin::ParameterLayout loadParameterLayout (const File& file);

template <FileType Type = FileType::JSON>
bool saveParameterLayout (const plugin::ParameterLayout& layout, const File& file);

}  // namespace lemons::files
