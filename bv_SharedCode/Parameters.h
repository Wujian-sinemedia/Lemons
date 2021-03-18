
namespace bav
{
    
    /*
     this function attempts to return the default location your plugin's preset files should be saved to and loaded from.
     if the directory cannot be found for your plugin, calling this function will attempt to create it.
     */
    
    inline juce::File getPresetsFolder (std::string companyName, std::string pluginName)
    {
        juce::File rootFolder;
        
#if (BV_OSX || BV_POSIX || BV_IOS)
        rootFolder = juce::File::getSpecialLocation (juce::File::SpecialLocationType::userApplicationDataDirectory)
                        .getChildFile ("Audio")
                        .getChildFile ("Presets")
                        .getChildFile (companyName)
                        .getChildFile (pluginName);
#elif (BV_LINUX || BV_BSD || BV_ANDROID)
        rootFolder = juce::File::getSpecialLocation (juce::File::SpecialLocationType::userApplicationDataDirectory)
                        .getChildFile (companyName)
                        .getChildFile (pluginName);
#elif BV_WINDOWS
        rootFolder = juce::File::getSpecialLocation (juce::File::SpecialLocationType::userDocumentsDirectory)
                        .getChildFile (companyName)
                        .getChildFile (pluginName);
#else
  #error Unknown operating system!
#endif
        
        if (! rootFolder.isDirectory())
            rootFolder.createDirectory(); // creates the presets folder if it doesn't already exist
        
        return rootFolder;
    }
    
    
    /*
        This function returns a given RangedAudioParameter's current normalized value in the range 0.0f to 1.0f.
        This function is necessary because the provided juce subclasses AudioParameterFloat|Bool|Int implement getValue() as private.
    */
    inline float getNormalizedParameterValue (juce::RangedAudioParameter& param)
    {
        return param.getValue();
    }
    
    
}  // namespace bav
