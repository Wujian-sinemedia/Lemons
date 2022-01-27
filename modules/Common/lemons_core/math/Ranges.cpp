namespace lemons::ranges
{

template <>
ParameterRange create (float minimum, float maximum)
{
	return { minimum, maximum, 0.01f };
}

template <>
ParameterRange create (double minimum, double maximum)
{
	return create (static_cast<float> (minimum),
				   static_cast<float> (maximum));
}

template <>
ParameterRange create (int minimum, int maximum)
{
	auto denormalize = [] (float start, float end, float v)
	{
		return juce::jlimit (start, end,
							 v * (end - start) + start);
	};

	auto normalize = [] (float start, float end, float v)
	{
		return juce::jlimit (0.f, 1.f,
							 (v - start) / (end - start));
	};

	auto snap = [] (float start, float end, float v)
	{
		return static_cast<float> (juce::roundToInt (juce::jlimit (start, end, v)));
	};

	ParameterRange rangeWithInterval { static_cast<float> (minimum), static_cast<float> (maximum), std::move (denormalize), std::move (normalize), std::move (snap) };

	rangeWithInterval.interval = 1.f;

	return rangeWithInterval;
}

template <>
ParameterRange create (bool, bool)
{
	return createBool();
}

ParameterRange createBool()
{
	return { 0.f, 1.f, 1.f };
}


namespace NormRangeVTProps
{
static constexpr auto startProp			= "minimum";
static constexpr auto endProp			= "maximum";
static constexpr auto intervalProp		= "interval";
static constexpr auto skewProp			= "skew";
static constexpr auto symmetricSkewProp = "symmetric_skew";
}  // namespace NormRangeVTProps

template <typename ValueType>
ValueTree toValueTree (const Range<ValueType>& range)
{
	using namespace NormRangeVTProps;

	ValueTree tree { valueTreeType };

	tree.setProperty (startProp, range.start, nullptr);
	tree.setProperty (endProp, range.end, nullptr);
	tree.setProperty (intervalProp, range.interval, nullptr);
	tree.setProperty (skewProp, range.skew, nullptr);
	tree.setProperty (symmetricSkewProp, range.symmetricSkew, nullptr);

	return tree;
}

template ValueTree toValueTree (const Range<float>&);
template ValueTree toValueTree (const Range<int>&);
template ValueTree toValueTree (const Range<double>&);


template <typename ValueType>
Range<ValueType> fromValueTree (const ValueTree& tree)
{
	using namespace NormRangeVTProps;

	if (! tree.hasType (valueTreeType))
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

template Range<float>  fromValueTree (const ValueTree&);
template Range<int>	   fromValueTree (const ValueTree&);
template Range<double> fromValueTree (const ValueTree&);

}  // namespace lemons::ranges
