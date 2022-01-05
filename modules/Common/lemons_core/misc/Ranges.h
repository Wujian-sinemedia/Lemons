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
