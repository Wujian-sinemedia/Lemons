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


/** @name lemons_norm_range_serializing NormalisableRange objects
    @ingroup lemons_core
 */
///@{

template <typename ValueType>
[[nodiscard]] ValueTree rangeToValueTree (const juce::NormalisableRange<ValueType>& range);

template <typename ValueType>
[[nodiscard]] juce::NormalisableRange<ValueType> valueTreeToRange (const ValueTree& tree);

///@}


/** @name lemons_memory_blocks_serializing MemoryBlocks
    @ingroup lemons_core
 */
///@{

/** Converts a MemoryBlock to a String representation.
    @see memoryBlockFromString()
 */
[[nodiscard]] String memoryBlockToString (const MemoryBlock& block);

/** Returns a MemoryBlock from a String representation of one.
    @see memoryBlockToString()
 */
[[nodiscard]] MemoryBlock memoryBlockFromString (const String& string);

///@}


/** @name lemons_vt_json_conversion ValueTree/JSON conversion
    @ingroup lemons_core
 */
///@{

/** Converts a ValueTree to a JSON string.
    @see valueTreeFromJSON()
 */
[[nodiscard]] String valueTreeToJSON (const ValueTree& v);

/** Converts a JSON string to a ValueTree. If the JSON cannpt be parsed correctly, a null ValueTree will be returned.
    @see valueTreeToJSON()
 */
[[nodiscard]] ValueTree valueTreeFromJSON (const String& jsonText);

///@}

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
