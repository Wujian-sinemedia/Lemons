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

#include <juce_core/juce_core.h>

namespace lemons::locale
{

using juce::String;
using juce::StringArray;

/** Converts the name of a language to an ISO639-1 or ISO639-2 language code.
    @param name The name of the language. If the passed language name cannot be located in the list of known languages, an empty string will be returned.
    @param asISO639_2 When true, the returned language code will be a 3-character ISO639-2 code. When false, the returned language code will be a 2-character ISO639-1 code. Note that not all known languages have ISO639-1 codes, as some languages were added to the ISO639-2 extension of the standard.
    @see countryCodeToLanguageName()
 */
[[nodiscard]] String languageNameToCode (const String& name, bool asISO639_2 = true);


/** Converts an ISO language code to the name of the language it represents.
    If the passed string is 2 characters long, it will be interpreted as an ISO639-1 code; if it is 3 characters long, it will be interpreted as an ISO639-2 code. Other lengths of passed strings are invalid and will cause this method to return an empty string.
    If the language cannot be identified in the list of known languages, an empty string will be returned.
    @see languageNameToCountryCode()
 */
[[nodiscard]] String languageCodeToName (const String& countryCode);


/** Returns a list of 2-character ISO 3166 country codes where the specified language is an officially recognized language, or a significant minority language.
    These country codes are referenced from https://en.wikipedia.org/wiki/List_of_ISO_3166_country_codes.
    @attention This feature is mainly intended for generating translation files, not for selecting an appropriate language at runtime; I recommend choosing the language based on the user's display language settings, which can be obtained using juce::SystemStats::getDisplayLanguage() (which is what initializeDefaultTranslations() does under the hood).
 */
[[nodiscard]] StringArray getCountryCodesForLanguage (const String& languageName);


/** Returns a list of language names that are officially recognized or are significant minority languages for the specified country.
    @param countryCode A 2-character ISO 3166 country code. If an invalid country code is passed, this method will return an empty StringArray.
    @attention This feature is mainly intended for generating translation files, not for selecting an appropriate language at runtime; I recommend choosing the language based on the user's display language settings, which can be obtained using juce::SystemStats::getDisplayLanguage() (which is what initializeDefaultTranslations() does under the hood).
 */
[[nodiscard]] StringArray getLanguagesForCountry (const String& countryCode);

}  // namespace lemons::locale
