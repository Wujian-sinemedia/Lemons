/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 *  ======================================================================================
 */

namespace lemons::locale
{

void initializeTranslations (const binary::Data& data,
							 bool				 ignoreCaseOfKeys)
{
	if (! data.isValid())
		return;

	juce::LocalisedStrings::setCurrentMappings (new juce::LocalisedStrings (data.getAsString(), ignoreCaseOfKeys));
}


[[nodiscard]] String getLanguageToUse()
{
	const auto countryCode = juce::SystemStats::getDisplayLanguage().upToFirstOccurrenceOf ("-", false, false);

	return languageCodeToName (countryCode);
}


static constexpr auto TRANSLATION_FILE_PREFIX = "trans_";
static constexpr auto TRANSLATION_FILE_XTN	  = ".txt";


void initializeDefaultTranslations()
{
	const auto language = getLanguageToUse();

	if (language.isEmpty())
		return;

	const binary::Data translationData { TRANSLATION_FILE_PREFIX + language + TRANSLATION_FILE_XTN };

	initializeTranslations (translationData);
}


TranslationsInitializer::TranslationsInitializer()
{
	initializeDefaultTranslations();
}

}  // namespace lemons::locale
