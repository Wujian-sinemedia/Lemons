
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


struct TranslationInitializer
{
    TranslationInitializer (const juce::File& translationFile,
                            bool              ignoreCaseOfKeys = true)
    {
        initializeTranslations (translationFile, ignoreCaseOfKeys);
    }
};


}  // namespace bav
