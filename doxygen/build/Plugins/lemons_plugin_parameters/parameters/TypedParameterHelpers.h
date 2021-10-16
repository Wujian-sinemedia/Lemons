
/** @ingroup parameters
 *  @{
 */
#pragma once

namespace lemons::plugin
{

template < typename ValueType >
inline juce::NormalisableRange< float > createRange (ValueType minimum, ValueType maximum);

template <>
inline juce::NormalisableRange< float > createRange (float minimum, float maximum)
{
    return {minimum, maximum, 0.01f};
}

template <>
inline juce::NormalisableRange< float > createRange (int minimum, int maximum)
{
    juce::NormalisableRange< float > rangeWithInterval {(float) minimum, (float) maximum,
                                                        [] (float start, float end, float v)
                                                        { return juce::jlimit (start, end, v * (end - start) + start); },
                                                        [] (float start, float end, float v)
                                                        { return juce::jlimit (0.f, 1.f, (v - start) / (end - start)); },
                                                        [] (float start, float end, float v)
                                                        { return static_cast< float > (juce::roundToInt (juce::jlimit (start, end, v))); }};
    rangeWithInterval.interval = 1.f;
    return rangeWithInterval;
}

template <>
inline juce::NormalisableRange< float > createRange (bool, bool)
{
    return {0.f, 1.f, 1.f};
}

/*-------------------------------------------------------------------------------------------------------*/

template < typename ValueType >
inline std::function< String (ValueType, int) > createDefaultStringFromValueFunc (float rangeInterval);

template <>
inline std::function< String (float, int) > createDefaultStringFromValueFunc (float rangeInterval)
{
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
inline std::function< String (int, int) > createDefaultStringFromValueFunc (float)
{
    return [] (int v, int num) -> String
    { return num > 0 ? String (v).substring (0, num) : String (v); };
}

template <>
inline std::function< String (bool, int) > createDefaultStringFromValueFunc (float)
{
    return [] (bool v, int) -> String
    { return v ? TRANS ("On") : TRANS ("Off"); };
}

/*-------------------------------------------------------------------------------------------------------*/

template < typename ValueType >
inline std::function< ValueType (const String&) > createDefaultValueFromStringFunc();

template <>
inline std::function< float (const String&) > createDefaultValueFromStringFunc()
{
    return [] (const String& text) -> float
    { return text.getFloatValue(); };
}

template <>
inline std::function< int (const String&) > createDefaultValueFromStringFunc()
{
    return [] (const String& text) -> int
    { return text.getIntValue(); };
}

template <>
inline std::function< bool (const String&) > createDefaultValueFromStringFunc()
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

/*-------------------------------------------------------------------------------------------------------*/

template < typename ValueType >
inline std::function< String (float) > convertValToStringFunc (std::function< String (ValueType, int) > origFunc)
{
    if (origFunc == nullptr)
        origFunc = createDefaultStringFromValueFunc< ValueType > (1.f);

    return [=] (float value) -> String
    { return origFunc (static_cast< ValueType > (value), 0); };
}


template < typename ValueType >
inline std::function< float (const String&) > convertStringToValFunc (std::function< ValueType (const String&) > origFunc)
{
    if (origFunc == nullptr)
        origFunc = createDefaultValueFromStringFunc< ValueType >();

    return [=] (const String& text) -> float
    { return static_cast< float > (origFunc (text)); };
}

}  // namespace lemons::plugin

/** @}*/
