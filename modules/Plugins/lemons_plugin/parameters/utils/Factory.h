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

/** Creates the appropriate kind of TypedParameter for the passed ParameterTraits object.
 */
std::unique_ptr<Parameter> createParameter (const ParameterTraits& traits)
{
	if (traits.valueType == ParameterTraits::ValueType::floatValue)
	{
		return std::make_unique<TypedParameter<float>> (traits);
	}

	if (traits.valueType == ParameterTraits::ValueType::intValue)
	{
		return std::make_unique<TypedParameter<int>> (traits);
	}

	jassert (traits.valueType == ParameterTraits::ValueType::boolValue);

	return std::make_unique<BoolParameter> (traits);
}

}  // namespace lemons::plugin
