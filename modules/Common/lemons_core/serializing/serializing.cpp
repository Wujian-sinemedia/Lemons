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

namespace NormRangeVTProps
{
static constexpr auto startProp         = "minimum";
static constexpr auto endProp           = "maximum";
static constexpr auto intervalProp      = "interval";
static constexpr auto skewProp          = "skew";
static constexpr auto symmetricSkewProp = "symmetric_skew";
}  // namespace NormRangeVTProps

template <typename ValueType>
ValueTree rangeToValueTree (const juce::NormalisableRange<ValueType>& range)
{
	using namespace NormRangeVTProps;

	ValueTree tree { rangeTreeType };

	tree.setProperty (startProp, range.start, nullptr);
	tree.setProperty (endProp, range.end, nullptr);
	tree.setProperty (intervalProp, range.interval, nullptr);
	tree.setProperty (skewProp, range.skew, nullptr);
	tree.setProperty (symmetricSkewProp, range.symmetricSkew, nullptr);

	return tree;
}

template ValueTree rangeToValueTree (const juce::NormalisableRange<float>&);
template ValueTree rangeToValueTree (const juce::NormalisableRange<int>&);
template ValueTree rangeToValueTree (const juce::NormalisableRange<double>&);


template <typename ValueType>
juce::NormalisableRange<ValueType> valueTreeToRange (const ValueTree& tree)
{
	using namespace NormRangeVTProps;

	if (! tree.hasType (rangeTreeType))
		return {};

	if (tree.hasProperty (startProp))
	{
		const auto start = (ValueType) tree.getProperty (startProp);

		if (tree.hasProperty (endProp))
		{
			const auto end = (ValueType) tree.getProperty (endProp);

			if (! tree.hasProperty (intervalProp))
				return { start, end };

			const auto interval = (ValueType) tree.getProperty (intervalProp);

			if (! tree.hasProperty (skewProp))
				return { start, end, interval };

			const auto skew = (ValueType) tree.getProperty (skewProp);

			if (! tree.hasProperty (symmetricSkewProp))
				return { start, end, interval, skew };

			return { start, end, interval, skew, (bool) tree.getProperty (symmetricSkewProp) };
		}
	}

	return {};
}

template juce::NormalisableRange<float>  valueTreeToRange (const ValueTree&);
template juce::NormalisableRange<int>    valueTreeToRange (const ValueTree&);
template juce::NormalisableRange<double> valueTreeToRange (const ValueTree&);

/*-------------------------------------------------------------------------------------------------------------------------------------*/


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
