
namespace bav::plugin
{
PresetsLibrary::PresetsLibrary (StateBase&    stateToUse,
                                const String& companyNameToUse,
                                const String& productNameToUse,
                                const String& presetFileExtensionToUse,
                                UndoManager*  um)
    : localPresets (stateToUse, companyNameToUse, productNameToUse, presetFileExtensionToUse, um)
{
    rescanPresets();
}

const juce::Array< Preset >& PresetsLibrary::getPresets()
{
    rescanPresets();
    return presets;
}

void PresetsLibrary::rescanPresets()
{
    presets.clearQuick();

    presets.addArray (localPresets.getPresets());
    presets.addArray (factoryPresets.getPresets());
    presets.addArray (databasePresets.getPresets());
}

}  // namespace bav::plugin
