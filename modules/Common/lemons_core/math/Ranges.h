/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

#pragma once

#include <juce_data_structures/juce_data_structures.h>

namespace lemons::ranges
{

using juce::ValueTree;

template <typename ValueType>
using Range = juce::NormalisableRange<ValueType>;

using ParameterRange = Range<float>;


template <typename ValueType>
[[nodiscard]] ParameterRange create (ValueType minimum, ValueType maximum);

[[nodiscard]] ParameterRange createBool();


static constexpr auto valueTreeType = "NormalisableRange";

/** @defgroup lemons_serializing Serializing
	@ingroup lemons_core
	Utilities for serializing.
 */

/** @ingroup lemons_serializing
 */
template <typename ValueType>
[[nodiscard]] ValueTree toValueTree (const Range<ValueType>& range);

/** @ingroup lemons_serializing
 */
template <typename ValueType>
[[nodiscard]] Range<ValueType> fromValueTree (const ValueTree& tree);

}  // namespace lemons::ranges

namespace juce
{

#define LEMONS_CREATE_NORM_RANGE_CONVERTER(Type)                                                   \
	template <>                                                                                    \
	struct VariantConverter<NormalisableRange<Type>>                                               \
	{                                                                                              \
		static NormalisableRange<Type> fromVar (const var& v)                                      \
		{                                                                                          \
			return lemons::ranges::fromValueTree<Type> (VariantConverter<ValueTree>::fromVar (v)); \
		}                                                                                          \
		static var toVar (const NormalisableRange<Type>& r)                                        \
		{                                                                                          \
			return VariantConverter<ValueTree>::toVar (lemons::ranges::toValueTree (r));           \
		}                                                                                          \
	};

LEMONS_CREATE_NORM_RANGE_CONVERTER (float)
LEMONS_CREATE_NORM_RANGE_CONVERTER (int)
LEMONS_CREATE_NORM_RANGE_CONVERTER (double)

#undef LEMONS_CREATE_NORM_RANGE_CONVERTER

}  // namespace juce
