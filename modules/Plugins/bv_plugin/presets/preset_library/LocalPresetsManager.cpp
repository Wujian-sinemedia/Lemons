
namespace bav::plugin
{
LocalPresetsManager::LocalPresetsManager (StateBase&    stateToUse,
                                          const String& companyNameToUse,
                                          const String& productNameToUse,
                                          const String& presetFileExtensionToUse,
                                          UndoManager*  um)
    : companyName (companyNameToUse), productName (productNameToUse), presetFileExtension (presetFileExtensionToUse), undo (um), state (stateToUse)
{
    rescanPresetsFolder();
}

static inline File get_preset_root_folder()
{
#if JUCE_WINDOWS
    return File::getSpecialLocation (File::SpecialLocationType::userDocumentsDirectory);
#endif

    auto rootFolder = File::getSpecialLocation (File::SpecialLocationType::userApplicationDataDirectory);

#if JUCE_MAC
    rootFolder = rootFolder.getChildFile ("Audio").getChildFile ("Presets");
#endif

    return rootFolder;
}

File LocalPresetsManager::presetsFolder()
{
    auto rootFolder = get_preset_root_folder().getChildFile (companyName).getChildFile (productName);

    if (! rootFolder.isDirectory())
        rootFolder.createDirectory();

    return rootFolder;
}

void LocalPresetsManager::rescanPresetsFolder()
{
    presets.clearQuick();

    for (auto entry : juce::RangedDirectoryIterator (presetsFolder(), true))
    {
        auto file = entry.getFile();
        if (! file.existsAsFile()) continue;

        presets.add (serializing::fromJSONCreate< Preset > (file));
    }
}

const juce::Array< Preset >& LocalPresetsManager::getPresets()
{
    rescanPresetsFolder();
    return presets;
}

}  // namespace bav::plugin
