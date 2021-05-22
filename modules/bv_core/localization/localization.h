
namespace bav
{
//static inline juce::String getCountryCodeToUse()
//{
//
//}


static inline void initializeTranslations (const juce::File& translationFile,
                                           bool              ignoreCaseOfKeys = true)
{
    if (translationFile.existsAsFile())
        juce::LocalisedStrings::setCurrentMappings (
            new juce::LocalisedStrings (translationFile, ignoreCaseOfKeys));
}


}  // namespace bav
