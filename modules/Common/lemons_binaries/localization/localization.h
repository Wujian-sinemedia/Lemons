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
[[nodiscard]] juce::StringArray getCountryCodesForLanguage (const String& languageName);


/** Initializes the juce::LocalisedStrings translation system from a translation file contained in a binary::Data object. */
void initializeTranslations (const binary::Data& data, bool ignoreCaseOfKeys = true);


/** Attempts to load a set of default translations from binary data, chosen based on the user's display language settings. */
void initializeDefaultTranslations();


/** A struct whose constructor simply calls initializeDefaultTranslations().
    This is useful for initializing the translations system as part of a base class, before the other members of your class are created; for example:
    @code
    struct MyProcessor : public juce::AudioProcessor, private lemons::locale::TranslationsInitializer
    {
        MyProcessor()
        {
            // parameters you create in the constructor can now call juce::translate() or TRANS() to internationalize their display names!
            // the translation system will be initialized before you enter the body of this constructor!
        }
    };
    @endcode
 */
struct TranslationsInitializer
{
	/** The constructor simply calls initializeDefaultTranslations(). */
	explicit TranslationsInitializer();

	/** Destructor. */
	virtual ~TranslationsInitializer() = default;
};

}  // namespace lemons::locale
