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


/** Converts a MemoryBlock to a String representation.
    @see memoryBlockFromString()
 */
[[nodiscard]] String memoryBlockToString (const MemoryBlock& block);

/** Returns a MemoryBlock from a String representation of one.
    @see memoryBlockToString()
 */
[[nodiscard]] MemoryBlock memoryBlockFromString (const String& string);


/** Converts a ValueTree to a JSON string.
    @see valueTreeFromJSON()
 */
[[nodiscard]] String valueTreeToJSON (const ValueTree& v);

/** Converts a JSON string to a ValueTree.
    @see valueTreeToJSON()
 */
[[nodiscard]] ValueTree valueTreeFromJSON (const String& jsonText);

}  // namespace lemons::serializing


/*---------------------------------------------------------------------------------------------------------------------------------*/

#if LEMONS_UNIT_TESTS

#  include <lemons_unit_testing/lemons_unit_testing.h>


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
