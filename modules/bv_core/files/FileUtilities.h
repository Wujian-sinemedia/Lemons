
#pragma once

namespace bav
{
/*
 this function attempts to return the default location your plugin's preset files should be saved to and loaded from.
 if the directory cannot be found for your plugin, calling this function will attempt to create it.
 */
extern juce::File getPresetsFolder (std::string companyName,
                                    std::string pluginName);


//==============================================================================


extern juce::String addFileExtensionIfMissing (const juce::String& string,
                                               const juce::String& extension);

extern juce::String removeFileExtensionIfThere (const juce::String& string,
                                                const juce::String& extension);


//==============================================================================


extern bool renameFile (juce::File& f, const juce::String& newName);


}  // namespace bav
