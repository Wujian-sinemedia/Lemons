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

void initializeTranslations (const binary::Data& data,
                             bool                ignoreCaseOfKeys)
{
    if (! data.isValid())
        return;
    
    juce::LocalisedStrings::setCurrentMappings (new juce::LocalisedStrings (data.getAsString(), ignoreCaseOfKeys));
}


static inline String getLanguageToUse()
{
	const auto countryCode = juce::SystemStats::getDisplayLanguage().upToFirstOccurrenceOf ("-", false, false);

	return countryCode;
}


static constexpr auto TRANSLATION_FILE_PREFIX = "trans_";
static constexpr auto TRANSLATION_FILE_XTN    = ".txt";


void initializeDefaultTranslations()
{
    const binary::Data translationData { TRANSLATION_FILE_PREFIX + getLanguageToUse() + TRANSLATION_FILE_XTN };
    
	initializeTranslations (translationData);
}


TranslationsInitializer::TranslationsInitializer()
{
	initializeDefaultTranslations();
}

}  // namespace lemons
