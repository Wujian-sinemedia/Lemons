
namespace lemons
{
static inline String getLanguageToUse()
{
    const auto countryCode = juce::SystemStats::getDisplayLanguage().upToFirstOccurrenceOf ("-", false, false);
    
    return countryCode;
}


static constexpr auto TRANSLATION_FILE_PREFIX = "trans_";
static constexpr auto TRANSLATION_FILE_XTN = ".txt";


static inline RawData getDefaultTranslationFile()
{
#if LEMONS_HAS_BINARY_DATA
	const auto languageToTranslationFileName = [] (const String& language) -> String
	{
		return TRANSLATION_FILE_PREFIX + language + TRANSLATION_FILE_XTN;
	};

    return RawData {languageToTranslationFileName (getLanguageToUse())};
#else
	return {};
#endif
}

void initializeTranslations (const File& translationFile, bool ignoreCaseOfKeys)
{
    if (translationFile.existsAsFile())
        juce::LocalisedStrings::setCurrentMappings (
                                                    new juce::LocalisedStrings (translationFile, ignoreCaseOfKeys));
}

void initializeTranslations (RawData data,
                                           bool    ignoreCaseOfKeys)
{
	if (! data.isValid())
		return;

	juce::LocalisedStrings::setCurrentMappings (
	    new juce::LocalisedStrings ({ juce::CharPointer_UTF8 (data.data), static_cast<size_t> (data.size) },
	                                ignoreCaseOfKeys));
}


void initializeDefaultTranslations()
{
	initializeTranslations (getDefaultTranslationFile());
}

}  // namespace lemons
