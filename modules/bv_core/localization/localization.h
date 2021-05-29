
#pragma once

namespace bav
{
static inline juce::String getCountryCodeToUse()
{
    return juce::SystemStats::getDisplayLanguage().upToFirstOccurrenceOf ("-", false, false);
}


static inline RawData getDefaultTranslationFile (juce::String defaultCountryCode = "en")
{
#if BV_HAS_BINARY_DATA
    const auto countryCodeToTranslationFileName = [] (const juce::String& countryCode)
    {
        return "trans_" + countryCode + ".txt";
    };

    const auto fileName = countryCodeToTranslationFileName (getCountryCodeToUse());
    auto       data     = RawData (fileName.toRawUTF8());

    if (data.isValid())
        return data;

    const auto defaultFilename = countryCodeToTranslationFileName (defaultCountryCode);
    return RawData (defaultFilename.toRawUTF8());
#else
    return RawData();
#endif
}


static inline void initializeTranslations (const juce::File& translationFile,
                                           bool              ignoreCaseOfKeys = true)
{
    if (translationFile.existsAsFile())
        juce::LocalisedStrings::setCurrentMappings (
            new juce::LocalisedStrings (translationFile, ignoreCaseOfKeys));
}


static inline void initializeTranslationsFromBinaryData (RawData data,
                                                         bool    ignoreCaseOfKeys = true)
{
    if (! data.isValid())
        return;

    juce::String fileContents (data.data, static_cast< size_t > (data.size));

    juce::LocalisedStrings::setCurrentMappings (
        new juce::LocalisedStrings (fileContents, ignoreCaseOfKeys));
}


static inline void initializeDefaultTranslations()
{
    initializeTranslationsFromBinaryData (getDefaultTranslationFile());
}


}  // namespace bav
