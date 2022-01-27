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

static inline String appendParamLabel (const String& orig, const String& paramLabel)
{
	if (paramLabel.isNotEmpty())
		return orig + " " + paramLabel;

	return orig;
}


BasicValToStringFunc createDefaultValueToTextFunction (const String& paramLabel)
{
	return [paramLabel] (float value) -> String
	{
		return appendParamLabel (String (value), paramLabel);
	};
}

BasicStringToValFunc createDefaultTextToValueFunction()
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
ValToStringFunc<ValueType> createDefaultStringFromValueFunc (float rangeInterval, const String& paramLabel)
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
ValToStringFunc<int> createDefaultStringFromValueFunc (float, const String& paramLabel)
{
	return [paramLabel] (int v, int num) -> String
	{
		return checkForLength (appendParamLabel (String (v), paramLabel),
							   num);
	};
}

template <>
ValToStringFunc<bool> createDefaultStringFromValueFunc (float, const String&)
{
	return [] (bool v, int maxLength) -> String
	{
		const auto text = v ? TRANS ("On") : TRANS ("Off");

		return checkForLength (text, maxLength);
	};
}


/*-------------------------------------------------------------------------------------------------------*/


template <typename ValueType>
StringToValFunc<ValueType> createDefaultValueFromStringFunc()
{
	static_assert (std::is_same_v<ValueType, float>, "");

	return [] (const String& text) -> float
	{ return text.getFloatValue(); };
}

template <>
StringToValFunc<int> createDefaultValueFromStringFunc()
{
	return [] (const String& text) -> int
	{ return text.getIntValue(); };
}

template <>
StringToValFunc<bool> createDefaultValueFromStringFunc()
{
	juce::StringArray onStrings;
	onStrings.add (TRANS ("on"));
	onStrings.add (TRANS ("yes"));
	onStrings.add (TRANS ("true"));

	juce::StringArray offStrings;
	offStrings.add (TRANS ("off"));
	offStrings.add (TRANS ("no"));
	offStrings.add (TRANS ("false"));

	return [onStrings, offStrings] (const String& text) -> bool
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
BasicValToStringFunc convertValToStringFuncFromTyped (ValToStringFunc<ValueType> origFunc,
													  const String&				 paramLabel,
													  float						 rangeInterval)
{
	if (origFunc == nullptr)
		origFunc = createDefaultStringFromValueFunc<ValueType> (rangeInterval, paramLabel);

	return [origFunc] (float value) -> String
	{ return origFunc (static_cast<ValueType> (value), 0); };
}

template BasicValToStringFunc convertValToStringFuncFromTyped (ValToStringFunc<float>, const String&, float);
template BasicValToStringFunc convertValToStringFuncFromTyped (ValToStringFunc<int>, const String&, float);
template BasicValToStringFunc convertValToStringFuncFromTyped (ValToStringFunc<bool>, const String&, float);

/*-------------------------------------------------------------------------------------------------------*/

template <typename ValueType>
BasicStringToValFunc convertStringToValFuncFromTyped (StringToValFunc<ValueType> origFunc)
{
	if (origFunc == nullptr)
		origFunc = createDefaultValueFromStringFunc<ValueType>();

	return [origFunc] (const String& text) -> float
	{ return static_cast<float> (origFunc (text)); };
}

template BasicStringToValFunc convertStringToValFuncFromTyped (StringToValFunc<float>);
template BasicStringToValFunc convertStringToValFuncFromTyped (StringToValFunc<int>);
template BasicStringToValFunc convertStringToValFuncFromTyped (StringToValFunc<bool>);

/*-------------------------------------------------------------------------------------------------------*/

template <typename ValueType>
ValToStringFunc<ValueType> convertValToStringFuncToTyped (BasicValToStringFunc origFunc, const String& paramLabel)
{
	if (origFunc == nullptr)
		origFunc = createDefaultValueToTextFunction (paramLabel);

	return [origFunc] (ValueType v, int maxLength)
	{
		return checkForLength (origFunc (static_cast<float> (v)),
							   maxLength);
	};
}

template ValToStringFunc<float> convertValToStringFuncToTyped (BasicValToStringFunc, const String&);
template ValToStringFunc<int>	convertValToStringFuncToTyped (BasicValToStringFunc, const String&);
template ValToStringFunc<bool>	convertValToStringFuncToTyped (BasicValToStringFunc, const String&);

/*-------------------------------------------------------------------------------------------------------*/

template <typename ValueType>
StringToValFunc<ValueType> convertStringToValFuncToTyped (BasicStringToValFunc origFunc)
{
	if (origFunc == nullptr)
		origFunc = createDefaultTextToValueFunction();

	return [origFunc] (const String& t)
	{ return static_cast<ValueType> (origFunc (t)); };
}

template StringToValFunc<float> convertStringToValFuncToTyped (BasicStringToValFunc);
template StringToValFunc<int>	convertStringToValFuncToTyped (BasicStringToValFunc);
template StringToValFunc<bool>	convertStringToValFuncToTyped (BasicStringToValFunc);

}  // namespace lemons::plugin::detail
