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

#include <juce_graphics/juce_graphics.h>


namespace lemons::serializing
{

using juce::MemoryBlock;
using juce::ValueTree;

static constexpr auto rangeTreeType = "NormalisableRange";

/** @defgroup lemons_serializing Serializing
    @ingroup lemons_core
    Utilities for serializing.
 */

/** @ingroup lemons_serializing
 */
template <typename ValueType>
[[nodiscard]] ValueTree rangeToValueTree (const juce::NormalisableRange<ValueType>& range);

/** @ingroup lemons_serializing
 */
template <typename ValueType>
[[nodiscard]] juce::NormalisableRange<ValueType> valueTreeToRange (const ValueTree& tree);


/** @ingroup lemons_serializing
    Converts a MemoryBlock to a String representation.
    @see memoryBlockFromString()
 */
[[nodiscard]] String memoryBlockToString (const MemoryBlock& block);

/** @ingroup lemons_serializing
    Returns a MemoryBlock from a String representation of one.
    @see memoryBlockToString()
 */
[[nodiscard]] MemoryBlock memoryBlockFromString (const String& string);


/** @ingroup lemons_serializing
    Converts a ValueTree to a JSON string.
    @see valueTreeFromJSON()
 */
[[nodiscard]] String valueTreeToJSON (const ValueTree& v);

/** @ingroup lemons_serializing
    Converts a JSON string to a ValueTree. If the JSON cannpt be parsed correctly, a null ValueTree will be returned.
    @see valueTreeToJSON()
 */
[[nodiscard]] ValueTree valueTreeFromJSON (const String& jsonText);


}  // namespace lemons::serializing


/*---------------------------------------------------------------------------------------------------------------------------------*/

#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

struct DataConversionTests : public CoreTest
{
	DataConversionTests();

private:
	void runTest() final;
};

static DataConversionTests dataConversionTest;

}  // namespace lemons::tests

#endif
