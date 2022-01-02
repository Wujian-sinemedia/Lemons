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


namespace lemons::plugin::detail
{

String paramNameToID (const String& name)
{
	return name.trim().replaceCharacter (' ', '_');
}

std::function<String (float)> createDefaultValueToTextFunction (const String& paramLabel)
{
	return [=] (float value) -> String
	{
		return String (value) + " " + paramLabel;
	};
}

std::function<float (const String&)> createDefaultTextToValueFunction()
{
	return [] (const String& text) -> float
	{
		return text.getFloatValue();
	};
}


/*-------------------------------------------------------------------------------------------------------*/


template <typename ValueType>
juce::NormalisableRange<float> createRange (ValueType minimum, ValueType maximum)
{
	static_assert (std::is_same_v<ValueType, float>, "");

	return { minimum, maximum, 0.01f };
}

template <>
juce::NormalisableRange<float> createRange (int minimum, int maximum)
{
	juce::NormalisableRange<float> rangeWithInterval { (float) minimum, (float) maximum,
		                                               [] (float start, float end, float v)
		                                               { return juce::jlimit (start, end, v * (end - start) + start); },
		                                               [] (float start, float end, float v)
		                                               { return juce::jlimit (0.f, 1.f, (v - start) / (end - start)); },
		                                               [] (float start, float end, float v)
		                                               { return static_cast<float> (juce::roundToInt (juce::jlimit (start, end, v))); } };
	rangeWithInterval.interval = 1.f;
	return rangeWithInterval;
}

template <>
juce::NormalisableRange<float> createRange (bool, bool)
{
	return { 0.f, 1.f, 1.f };
}


/*-------------------------------------------------------------------------------------------------------*/


template <typename ValueType>
std::function<String (ValueType, int)> createDefaultStringFromValueFunc (float rangeInterval)
{
	static_assert (std::is_same_v<ValueType, float>, "");

	return [rangeInterval] (float v, int length) -> String
	{
		const auto numDecimalPlaces = [rangeInterval]() -> int
		{
			int numPlaces = 7;

			if (rangeInterval != 0.f)
			{
				if (juce::approximatelyEqual (std::abs (rangeInterval - std::floor (rangeInterval)), 0.f))
					return 0;

				auto val = std::abs (juce::roundToInt (rangeInterval * std::pow (10, numPlaces)));

				while ((val % 10) == 0 && numPlaces > 0)
				{
					--numPlaces;
					val /= 10;
				}
			}

			return numPlaces;
		}();

		String asText (v, numDecimalPlaces);
		return length > 0 ? asText.substring (0, length) : asText;
	};
}

template <>
std::function<String (int, int)> createDefaultStringFromValueFunc (float)
{
	return [] (int v, int num) -> String
	{ return num > 0 ? String (v).substring (0, num) : String (v); };
}

template <>
std::function<String (bool, int)> createDefaultStringFromValueFunc (float)
{
	return [] (bool v, int) -> String
	{ return v ? TRANS ("On") : TRANS ("Off"); };
}


/*-------------------------------------------------------------------------------------------------------*/


template <typename ValueType>
std::function<ValueType (const String&)> createDefaultValueFromStringFunc()
{
	static_assert (std::is_same_v<ValueType, float>, "");

	return [] (const String& text) -> float
	{ return text.getFloatValue(); };
}

template <>
std::function<int (const String&)> createDefaultValueFromStringFunc()
{
	return [] (const String& text) -> int
	{ return text.getIntValue(); };
}

template <>
std::function<bool (const String&)> createDefaultValueFromStringFunc()
{
	juce::StringArray onStrings;
	onStrings.add (TRANS ("on"));
	onStrings.add (TRANS ("yes"));
	onStrings.add (TRANS ("true"));

	juce::StringArray offStrings;
	offStrings.add (TRANS ("off"));
	offStrings.add (TRANS ("no"));
	offStrings.add (TRANS ("false"));

	return [=] (const String& text) -> bool
	{
		const auto lowercaseText = text.toLowerCase();

		for (auto& testText : onStrings)
			if (lowercaseText == testText)
				return true;

		for (auto& testText : offStrings)
			if (lowercaseText == testText)
				return false;

		return text.getIntValue() > 0;
	};
}

}  // namespace lemons::plugin::detail
