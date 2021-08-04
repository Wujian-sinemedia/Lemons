
namespace bav::plugin
{
PresetManager::PresetManager (StateBase&    stateToUse,
                              const String& companyNameToUse,
                              const String& productNameToUse,
                              const String& presetFileExtensionToUse,
                              UndoManager*  um)
    : companyName (companyNameToUse), productName (productNameToUse), presetFileExtension (presetFileExtensionToUse), undo (um), state (stateToUse)
{
}

File PresetManager::presetsFolder()
{
    File rootFolder;

#if JUCE_WINDOWS
    rootFolder = File::getSpecialLocation (File::SpecialLocationType::userDocumentsDirectory);
#else
    rootFolder = File::getSpecialLocation (File::SpecialLocationType::userApplicationDataDirectory);

#    if JUCE_MAC
    rootFolder = rootFolder.getChildFile ("Audio").getChildFile ("Presets");
#    endif
#endif

    rootFolder = rootFolder.getChildFile (companyName).getChildFile (productName);

    if (! rootFolder.isDirectory())
        rootFolder.createDirectory();

    return rootFolder;
}

File PresetManager::presetNameToFilePath (const String& presetName)
{
    return presetsFolder().getChildFile (addFileExtensionIfMissing (presetName, presetFileExtension));
}

void PresetManager::savePreset (const String& presetName)
{
    serializing::toBinary (state, presetNameToFilePath (presetName));
    rescanPresetsFolder();
}

bool PresetManager::loadPreset (const String& presetName)
{
    auto file = presetNameToFilePath (presetName);

    if (! file.existsAsFile())
        return false;

    UndoManager::ScopedTransaction s {undo, TRANS ("Loaded preset") + " " + presetName};

    serializing::fromBinary (file, state);
    state.getParameters().refreshAllDefaults();
    presetLoaded.trigger();
    return true;
}

void PresetManager::deletePreset (const String& presetName)
{
    deleteFile (presetNameToFilePath (presetName));
    rescanPresetsFolder();
}

bool PresetManager::renamePreset (const String& previousName, const String& newName)
{
    auto file = presetNameToFilePath (previousName);

    if (! file.existsAsFile())
        return false;

    renameFile (file,
                addFileExtensionIfMissing (newName, presetFileExtension));

    rescanPresetsFolder();

    return true;
}

void PresetManager::rescanPresetsFolder()
{
    namesOfAvailablePresets.clearQuick();

    const auto len = static_cast< int > (presetFileExtension.length());

    for (auto entry : juce::RangedDirectoryIterator (presetsFolder(), true))
    {
        const auto filename = entry.getFile().getFileName();

        if (filename.endsWith (presetFileExtension))
            namesOfAvailablePresets.addIfNotAlreadyThere (filename.dropLastCharacters (len));
    }

    availablePresetsChanged.trigger();
}

const juce::StringArray& PresetManager::presetNames()
{
    rescanPresetsFolder();
    return namesOfAvailablePresets;
}

events::Broadcaster& PresetManager::getPresetsChangedBroadcaster()
{
    return availablePresetsChanged;
}

events::Broadcaster& PresetManager::getPresetLoadedBroadcaster()
{
    return presetLoaded;
}

}  // namespace bav::plugin
