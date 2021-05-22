
namespace bav
{
/*
 this function attempts to return the default location your plugin's preset files should be saved to and loaded from.
 if the directory cannot be found for your plugin, calling this function will attempt to create it.
 */
extern juce::File getPresetsFolder (std::string companyName,
                                           std::string pluginName)
{
    juce::File rootFolder;

#if JUCE_WINDOWS
    rootFolder = juce::File::getSpecialLocation (
        juce::File::SpecialLocationType::userDocumentsDirectory);
#else
    rootFolder = juce::File::getSpecialLocation (
        juce::File::SpecialLocationType::userApplicationDataDirectory);
#    if JUCE_MAC
    rootFolder = rootFolder.getChildFile ("Audio").getChildFile ("Presets");
#    endif
#endif
    rootFolder = rootFolder.getChildFile (companyName).getChildFile (pluginName);

    if (! rootFolder.isDirectory())
        rootFolder
            .createDirectory();  // creates the presets folder if it doesn't already exist

    return rootFolder;
}


//==============================================================================


extern juce::String addFileExtensionIfMissing (const juce::String& string,
                                                      const juce::String& extension)
{
    return juce::File::createLegalFileName (
        string.endsWith (extension) ? string.trim() : (string + extension).trim());
}


extern juce::String removeFileExtensionIfThere (const juce::String& string,
                                                       const juce::String& extension)
{
    return (string.endsWith (extension))
             ? string.dropLastCharacters (extension.length()).trim()
             : string.trim();
}


}  // namespace bav
