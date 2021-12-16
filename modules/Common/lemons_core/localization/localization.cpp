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

namespace lemons
{
static inline String getLanguageToUse()
{
	const auto countryCode = juce::SystemStats::getDisplayLanguage().upToFirstOccurrenceOf ("-", false, false);

	return countryCode;
}


static constexpr auto TRANSLATION_FILE_PREFIX = "trans_";
static constexpr auto TRANSLATION_FILE_XTN    = ".txt";


//static inline binary::Data getDefaultTranslationFile()
//{
//	const auto languageToTranslationFileName = [] (const String& language) -> String
//	{
//		return TRANSLATION_FILE_PREFIX + language + TRANSLATION_FILE_XTN;
//	};
//
//	return binary::Data { languageToTranslationFileName (getLanguageToUse()) };
//}

void initializeTranslations (const File& translationFile, bool ignoreCaseOfKeys)
{
	if (translationFile.existsAsFile())
		juce::LocalisedStrings::setCurrentMappings (
		    new juce::LocalisedStrings (translationFile, ignoreCaseOfKeys));
}

//void initializeTranslations (const binary::Data& data,
//                             bool                ignoreCaseOfKeys)
//{
//	if (! data.isValid())
//		return;
//
//	juce::LocalisedStrings::setCurrentMappings (new juce::LocalisedStrings (data.getAsString(), ignoreCaseOfKeys));
//}


void initializeDefaultTranslations()
{
	//initializeTranslations (getDefaultTranslationFile());
}


TranslationsInitializer::TranslationsInitializer()
{
	initializeDefaultTranslations();
}

}  // namespace lemons
