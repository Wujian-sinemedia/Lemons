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


/** An ephemeral type-erased way to store all the metadata necessary to reconstruct any given parameter object.
    You can use this type to save and later recreate a parameter object.
    @see ParameterLayout, ParameterList, Parameter, TypedParameter
 */
struct ParameterTraits final
{
    /** Creates a default, invalid ParameterTraits object. */
	explicit ParameterTraits() = default;

    /** Creates a ParameterTraits object with similar semantics as the TypedParameter constructor. */
	template <typename ValType>
	explicit ParameterTraits (ValType minimum, ValType maximum, ValType defaultVal,
	                          const String& nameToUse, const String& labelToUse,
	                          ValToStringFunc<ValType> stringFromValue = nullptr,
	                          StringToValFunc<ValType> valueFromString = nullptr,
	                          bool automatable = true, bool metaParameter = false,
	                          ParameterCategory categoryToUse = ParameterCategory::genericParameter);

    /** Saves this ParameterTraits object to a ValueTree. */
	[[nodiscard]] ValueTree toValueTree() const;

    /** Returns a ParameterTraits object from a state saved in a ValueTree. */
	[[nodiscard]] static ParameterTraits fromValueTree (const ValueTree& tree);

	/** @internal */
	static constexpr auto valueTreeType = "ParameterTraits";

    /** Returns true if this ParameterTraits object represents a valid call to the constructor of a parameter object. */
	[[nodiscard]] bool isValid() const;

    /** Creates a parameter object of the correct type for this ParameterTraits object.
        If this ParameterTraits represents a meta parameter, then the returned object will be a kind of MetaParameter.
        If this ParameterTraits represents a meter parameter, then the returned object will be a kind of MeterParameter.
        Otherwise, the returned object will be a kind of TypedParameter.
        The parameter's value type is determined by the ValueType enum.
     */
	[[nodiscard]] std::unique_ptr<Parameter> createParameter (const ParameterList& list) const;

    /** Enum to describe the various types of TypedParameter that can be instantiated. */
	enum class ValueType
	{
        /** Represents TypedParameter<float>/.*/
		floatValue,
        
        /** Represents TypedParameter<int>/.*/
		intValue,
        
        /** Represents TypedParameter<bool>/.*/
		boolValue
	};

    /** The value type of the parameter this ParameterTraits object represents. */
	ValueType valueType { ValueType::floatValue };

    /** The name of the parameter this ParameterTrait object represents. */
	String name;
    
    /** The label of the parameter this ParameterTrait object represents. */
	String label;

    /** The range of the parameter this ParameterTrait object represents. */
	ParameterRange range;
    
    /** The default value (in the normalized range 0-1) of the parameter this ParameterTrait object represents. */
	float defaultValue { 1.f };

    /** The value to text function of the parameter this ParameterTrait object represents. */
	BasicValToStringFunc valueToText { detail::createDefaultValueToTextFunction (label) };
    
    /** The text to value function of the parameter this ParameterTrait object represents. */
	BasicStringToValFunc textToValue { detail::createDefaultTextToValueFunction() };

    /** Whether this ParameterTraits' parameter object is automatable by DAWs. */
	bool isAutomatable { true };
    
    /** Whether this ParameterTraits' parameter object is a "meta parameter" -- that is, a parameter that controls the values of other parameters. */
	bool isMetaParameter { false };

    /** The category of the parameter this ParameterTrait object represents. */
	ParameterCategory category { ParameterCategory::genericParameter };
};


/** A container of ParameterTraits objects that can be used to recreate an entire ParameterList.
    @see ParameterTraits, ParameterList, Parameter
 */
struct ParameterLayout final
{
    /** Creates a default, empty parameter layout. */
	explicit ParameterLayout() = default;

    /** Reconstructs a ParameterLayout from a state saved in a ValueTree. */
	[[nodiscard]] static ParameterLayout fromValueTree (const ValueTree& tree);

    /** Saves this ParameterLayout to a ValueTree. */
	[[nodiscard]] ValueTree saveToValueTree() const;

    /** The parameter metadata. */
	std::vector<ParameterTraits> parameters;

	/** @internal */
	static constexpr auto valueTreeType = "ParameterLayout";
};

}  // namespace lemons::plugin


namespace lemons::binary
{

/** Loads a ParameterLayout from a file that was serialized in the given format. The default format is JSON. */
template <files::FileType Type = files::FileType::JSON>
[[nodiscard]] plugin::ParameterLayout getParameterLayout (const String& fileName);

}


namespace lemons::files
{

/** @name ParameterLayout */
///@{

/** Loads a ParameterLayout from a file that was serialized in the given format. The default format is JSON. */
template <FileType Type = FileType::JSON>
[[nodiscard]] plugin::ParameterLayout loadParameterLayout (const File& file);

/** Saves a ParameterLayout to a file in the given format. The default format is JSON. */
template <FileType Type = FileType::JSON>
bool saveParameterLayout (const plugin::ParameterLayout& layout, const File& file);

///@}

}  // namespace lemons::files
