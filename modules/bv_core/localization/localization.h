
namespace bav
{

static inline juce::String getCountryCodeToUse()
{
    return {};
}


static inline RawData getDefaultTranslationFile()
{
#if BV_HAS_BINARY_DATA
    juce::String fileName = "trans_" + getCountryCodeToUse() + ".txt";
    return RawData (fileName.toRawUTF8());
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
                                                         bool ignoreCaseOfKeys = true)
{
    if (! data.isValid())
        return;
    
    juce::String fileContents (data.data, static_cast<size_t>(data.size));
    
    juce::LocalisedStrings::setCurrentMappings (
                                                new juce::LocalisedStrings (fileContents, ignoreCaseOfKeys));
}


}  // namespace bav
