
namespace bav
{

static inline juce::String getCountryCodeToUse()
{
    return {};
}


static inline juce::File getDefaultTranslationFile()
{
#if BV_HAS_BINARY_DATA
    juce::String fileName = "trans_" + getCountryCodeToUse() + ".txt";
    RawData file (fileName.toRawUTF8());
#else
    return juce::File();
#endif
}


static inline void initializeTranslations (const juce::File& translationFile,
                                           bool              ignoreCaseOfKeys = true)
{
    if (translationFile.existsAsFile())
        juce::LocalisedStrings::setCurrentMappings (
            new juce::LocalisedStrings (translationFile, ignoreCaseOfKeys));
}


}  // namespace bav
