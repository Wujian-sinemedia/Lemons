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


namespace lemons::locale
{

using juce::String;
using juce::StringArray;

/** @defgroup lemons_locale Localization
    @ingroup lemons_core
    Utilities for internationalization.
 */


/** @name lemons_iso_language_codes ISO language codes
    @ingroup lemons_locale
 */
///@{

/** Converts the name of a language to an ISO639-1 or ISO639-2 language code.
    @param name The name of the language. If the passed language name cannot be located in the list of known languages, an empty string will be returned.
    @param asISO639_2 When true, the returned language code will be a 3-character ISO639-2 code. When false, the returned language code will be a 2-character ISO639-1 code. Note that not all known languages have ISO639-1 codes, as some languages were added to the ISO639-2 extension of the standard.
    @see languageCodeToName()
 */
[[nodiscard]] String languageNameToCode (const String& name, bool asISO639_2 = true);


/** Converts an ISO language code to the name of the language it represents.
    If the passed string is 2 characters long, it will be interpreted as an ISO639-1 code; if it is 3 characters long, it will be interpreted as an ISO639-2 code. Other lengths of passed strings are invalid and will cause this method to return an empty string.
    If the language cannot be identified in the list of known languages, an empty string will be returned.
    @see languageNameToCode()
 */
[[nodiscard]] String languageCodeToName (const String& languageCode);

///@}


/** @name lemons_iso_country_codes ISO country codes
    @ingroup lemons_locale
 */
///@{

/** Returns a list of 2-character ISO 3166 country codes where the specified language is an officially recognized language, a significant minority language, or a local indigenous language.
    These country codes are referenced from https://en.wikipedia.org/wiki/List_of_ISO_3166_country_codes.
    @attention This feature is mainly intended for generating translation files, not for selecting an appropriate language at runtime; I recommend choosing the language based on the user's display language settings, which can be obtained using juce::SystemStats::getDisplayLanguage().
    @param language The language name or language code. When interpreting this argument as a language code, a 2-character string will be interpreted as an ISO639-1 code; if it is 3 characters long, it will be interpreted as an ISO639-2 code. Other lengths of passed strings are invalid and will cause this method to return an empty string.
    @param languageName If true, the 'language' argument is interpreted as the name of the language; if false, it is interpreted as an ISO language code.
 */
[[nodiscard]] StringArray getCountryCodesForLanguage (const String& language, bool languageName = true);


/** Returns a list of language names that are officially recognized, significant minority languages, or local indigenous languages for the specified country.
    @param countryCode A 2-character ISO 3166 country code. If an invalid country code is passed, this method will return an empty StringArray.
    @attention This feature is mainly intended for generating translation files, not for selecting an appropriate language at runtime; I recommend choosing the language based on the user's display language settings, which can be obtained using juce::SystemStats::getDisplayLanguage().
    @see getLanguageCodesForCountry()
 */
[[nodiscard]] StringArray getLanguageNamesForCountry (const String& countryCode);


/** Returns a list of language codes for languages that are officially recognized, significant minority languages, or local indigenous languages for the specified country.
    @param countryCode A 2-character ISO 3166 country code. If an invalid country code is passed, this method will return an empty StringArray.
    @attention This feature is mainly intended for generating translation files, not for selecting an appropriate language at runtime; I recommend choosing the language based on the user's display language settings, which can be obtained using juce::SystemStats::getDisplayLanguage().
    @see getLanguageNamesForCountry()
 */
[[nodiscard]] StringArray getLanguageCodesForCountry (const String& countryCode, bool asISO639_2 = true);

///@}

}  // namespace lemons::locale
