
namespace bav
{
PresetManagerBase::PresetManagerBase (StateBase& stateToUse)
    : state (stateToUse)
{
    rescanPresetsFolder();
}

juce::File PresetManagerBase::presetsFolder()
{
    return getPresetsFolder (getCompanyName(), getProductName());
}

juce::File PresetManagerBase::presetNameToFilePath (const String& presetName)
{
    return presetsFolder().getChildFile (addFileExtensionIfMissing (presetName, getPresetFileExtension()));
}

void PresetManagerBase::savePreset (const String& presetName)
{
    serializing::toBinary (state, presetNameToFilePath (presetName));
    rescanPresetsFolder();
}

bool PresetManagerBase::loadPreset (const String& presetName)
{
    auto file = presetNameToFilePath (presetName);

    if (! file.existsAsFile())
        return false;

    serializing::fromBinary (file, state);
    state.refreshAllDefaults();
    return true;
}

void PresetManagerBase::deletePreset (const String& presetName)
{
    deleteFile (presetNameToFilePath (presetName));
    rescanPresetsFolder();
}

bool PresetManagerBase::renamePreset (const String& previousName, const String& newName)
{
    auto file = presetNameToFilePath (previousName);

    if (! file.existsAsFile())
        return false;

    renameFile (file,
                addFileExtensionIfMissing (newName, getPresetFileExtension()));

    rescanPresetsFolder();

    return true;
}

void PresetManagerBase::rescanPresetsFolder()
{
    namesOfAvailablePresets.clearQuick();

    const auto xtn = getPresetFileExtension();
    const auto len = static_cast< int > (xtn.length());

    for (auto entry : juce::RangedDirectoryIterator (presetsFolder(), true))
    {
        const auto filename = entry.getFile().getFileName();

        if (filename.endsWith (xtn))
            namesOfAvailablePresets.addIfNotAlreadyThere (filename.dropLastCharacters (len));
    }
    
    broadcaster.trigger();
}

const juce::StringArray& PresetManagerBase::presetNames()
{
    rescanPresetsFolder();
    return namesOfAvailablePresets;
}

}  // namespace bav
