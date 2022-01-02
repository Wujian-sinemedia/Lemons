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

static inline String appendParamLabel (const String& orig, const String& paramLabel)
{
	if (paramLabel.isNotEmpty())
		return orig + " " + paramLabel;

	return orig;
}


std::function<String (float)> createDefaultValueToTextFunction (const String& paramLabel)
{
	return [=] (float value) -> String
	{
		return appendParamLabel (String (value), paramLabel);
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

static inline String checkForLength (const String& orig, int maxLength)
{
	if (maxLength > 0)
		return orig.substring (0, maxLength);

	return orig;
}


template <typename ValueType>
std::function<String (ValueType, int)> createDefaultStringFromValueFunc (float rangeInterval, const String& paramLabel)
{
	static_assert (std::is_same_v<ValueType, float>, "");

	return [rangeInterval, paramLabel] (float v, int length) -> String
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

		return checkForLength (appendParamLabel (String (v, numDecimalPlaces), paramLabel),
		                       length);
	};
}

template <>
std::function<String (int, int)> createDefaultStringFromValueFunc (float, const String& paramLabel)
{
	return [paramLabel] (int v, int num) -> String
	{
		return checkForLength (appendParamLabel (String (v), paramLabel),
		                       num);
	};
}

template <>
std::function<String (bool, int)> createDefaultStringFromValueFunc (float, const String&)
{
	return [] (bool v, int maxLength) -> String
	{
		const auto text = v ? TRANS ("On") : TRANS ("Off");

		return checkForLength (text, maxLength);
	};
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

		for (const auto& testText : onStrings)
			if (lowercaseText == testText)
				return true;

		for (const auto& testText : offStrings)
			if (lowercaseText == testText)
				return false;

		return text.getIntValue() > 0;
	};
}

/*-------------------------------------------------------------------------------------------------------*/

template <typename ValueType>
std::function<String (float)> convertValToStringFuncFromTyped (std::function<String (ValueType, int)> origFunc, const String& paramLabel)
{
	if (origFunc == nullptr)
		origFunc = createDefaultStringFromValueFunc<ValueType> (1.f, paramLabel);

	return [=] (float value) -> String
	{ return origFunc (static_cast<ValueType> (value), 0); };
}

template std::function<String (float)> convertValToStringFuncFromTyped (std::function<String (float, int)>, const String&);
template std::function<String (float)> convertValToStringFuncFromTyped (std::function<String (int, int)>, const String&);
template std::function<String (float)> convertValToStringFuncFromTyped (std::function<String (bool, int)>, const String&);

/*-------------------------------------------------------------------------------------------------------*/

template <typename ValueType>
std::function<float (const String&)> convertStringToValFuncFromTyped (std::function<ValueType (const String&)> origFunc)
{
	if (origFunc == nullptr)
		origFunc = createDefaultValueFromStringFunc<ValueType>();

	return [=] (const String& text) -> float
	{ return static_cast<float> (origFunc (text)); };
}

template std::function<float (const String&)> convertStringToValFuncFromTyped (std::function<float (const String&)>);
template std::function<float (const String&)> convertStringToValFuncFromTyped (std::function<int (const String&)>);
template std::function<float (const String&)> convertStringToValFuncFromTyped (std::function<bool (const String&)>);

/*-------------------------------------------------------------------------------------------------------*/

template <typename ValueType>
std::function<String (ValueType, int)> convertValToStringFuncToTyped (std::function<String (float)> origFunc, const String& paramLabel)
{
	if (origFunc == nullptr)
		origFunc = createDefaultValueToTextFunction (paramLabel);

	return [=] (ValueType v, int maxLength)
	{
		return checkForLength (origFunc (static_cast<float> (v)),
		                       maxLength);
	};
}

template std::function<String (float, int)> convertValToStringFuncToTyped (std::function<String (float)>, const String&);
template std::function<String (int, int)>   convertValToStringFuncToTyped (std::function<String (float)>, const String&);
template std::function<String (bool, int)>  convertValToStringFuncToTyped (std::function<String (float)>, const String&);

/*-------------------------------------------------------------------------------------------------------*/

template <typename ValueType>
std::function<ValueType (const String&)> convertStringToValFuncToTyped (std::function<float (const String&)> origFunc)
{
	if (origFunc == nullptr)
		origFunc = createDefaultTextToValueFunction();

	return [=] (const String& t)
	{ return static_cast<ValueType> (origFunc (t)); };
}

template std::function<float (const String&)> convertStringToValFuncToTyped (std::function<float (const String&)>);
template std::function<int (const String&)>   convertStringToValFuncToTyped (std::function<float (const String&)>);
template std::function<bool (const String&)>  convertStringToValFuncToTyped (std::function<float (const String&)>);

}  // namespace lemons::plugin::detail
