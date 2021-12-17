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


namespace lemons::serializing
{

String memoryBlockToString (const MemoryBlock& block)
{
	return block.toBase64Encoding();
}

MemoryBlock memoryBlockFromString (const String& string)
{
	MemoryBlock block;
	block.fromBase64Encoding (string);
	return block;
}

String valueTreeToJSON (const ValueTree& tree)
{
	return juce::JSON::toString (juce::VariantConverter<ValueTree>::toVar (tree));
}

ValueTree valueTreeFromJSON (const String& jsonText)
{
	return juce::VariantConverter<ValueTree>::fromVar (juce::JSON::parse (jsonText));
}

}  // namespace lemons::serializing


/*-------------------------------------------------------------------------------------------------------------------------------------*/

#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

DataConversionTests::DataConversionTests()
    : CoreTest ("Data conversion functions")
{
}

void DataConversionTests::runTest()
{
	/*
	 - ValueTree to JSON
	 - ValueTree from JSON
	 */

	beginTest ("Memory block to/from string");

	//	const auto memStr     = serializing::memoryBlockToString (block);
	//	const auto memDecoded = serializing::memoryBlockFromString (memStr);
	//
	//	expect (block == memDecoded);
}

}  // namespace lemons::tests

#endif
