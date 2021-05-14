
namespace bav
{


/* Returns a unicode sharp symbol.
 */
static inline const juce::juce_wchar getSharpSymbol() noexcept   {   return *juce::CharPointer_UTF8 ("\xe2\x99\xaf");  }

/* Returns a unicode flat symbol.
 */
static inline const juce::juce_wchar getFlatSymbol() noexcept    {   return *juce::CharPointer_UTF8 ("\xe2\x99\xad");  }

/* Returns a unicode natural symbol.
 */
static inline const juce::juce_wchar getNaturalSymbol() noexcept {   return *juce::CharPointer_UTF8 ("\xe2\x99\xae");  }


//==============================================================================


/*
 this function attempts to return the default location your plugin's preset files should be saved to and loaded from.
 if the directory cannot be found for your plugin, calling this function will attempt to create it.
 */
static inline juce::File getPresetsFolder (std::string companyName, std::string pluginName)
{
    juce::File rootFolder;
    
#if JUCE_WINDOWS
    rootFolder = juce::File::getSpecialLocation (juce::File::SpecialLocationType::userDocumentsDirectory);
#else
    rootFolder = juce::File::getSpecialLocation (juce::File::SpecialLocationType::userApplicationDataDirectory);
  #if JUCE_MAC
    rootFolder = rootFolder.getChildFile ("Audio").getChildFile ("Presets");
  #endif
#endif
    rootFolder = rootFolder.getChildFile (companyName).getChildFile (pluginName);
    
    if (! rootFolder.isDirectory())
        rootFolder.createDirectory(); // creates the presets folder if it doesn't already exist
    
    return rootFolder;
}


//==============================================================================


static inline void initializeTranslations (const juce::File& translationFile,
                                           bool ignoreCaseOfKeys = true)
{
    if (translationFile.existsAsFile())
        juce::LocalisedStrings::setCurrentMappings (new juce::LocalisedStrings (translationFile, ignoreCaseOfKeys));
}


//==============================================================================


static inline juce::String addFileExtensionIfMissing (const juce::String& string, const juce::String& extension)
{
    return juce::File::createLegalFileName (string.endsWith (extension) ? string.trim() : (string + extension).trim());
}


static inline juce::String removeFileExtensionIfThere (const juce::String& string, const juce::String& extension)
{
    return (string.endsWith (extension)) ? string.dropLastCharacters (extension.length()).trim() : string.trim();
}

//==============================================================================


namespace DefaultValueTreeIds
{
#define BV_DECLARE_VALUETREEID(name) static inline juce::Identifier name { "name" }

    BV_DECLARE_VALUETREEID (ParameterGroupNode);
    BV_DECLARE_VALUETREEID (Parameter_Float);
    BV_DECLARE_VALUETREEID (Parameter_Int);
    BV_DECLARE_VALUETREEID (Parameter_Bool);
    BV_DECLARE_VALUETREEID (Parameter_Meter);
    BV_DECLARE_VALUETREEID (ParameterName);
    BV_DECLARE_VALUETREEID (ParameterValue);
    BV_DECLARE_VALUETREEID (ParameterDefaultValue);
    BV_DECLARE_VALUETREEID (ParameterIsChanging);

    BV_DECLARE_VALUETREEID (NonParameterGroupNode);
    BV_DECLARE_VALUETREEID (NonParameterNode_Int);
    BV_DECLARE_VALUETREEID (NonParameterNode_Bool);
    BV_DECLARE_VALUETREEID (NonParameterNode_Float);
    BV_DECLARE_VALUETREEID (NonParameterNode_String);
    BV_DECLARE_VALUETREEID (NonParameterName);
    BV_DECLARE_VALUETREEID (NonParameterValue);
    BV_DECLARE_VALUETREEID (NonParameterDefaultValue);

#undef BV_DECLARE_VALUETREEID
}


}  // namespace bav
