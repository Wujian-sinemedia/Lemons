
namespace bav::plugin
{
PresetsLibrary::PresetsLibrary (StateBase&    stateToUse,
                                const String& companyNameToUse,
                                const String& productNameToUse,
                                const String& presetFileExtensionToUse)
    : databasePresets (stateToUse, companyNameToUse, productNameToUse, presetFileExtensionToUse),
      factoryPresets (stateToUse, companyNameToUse, productNameToUse, presetFileExtensionToUse),
      localPresets (stateToUse, companyNameToUse, productNameToUse, presetFileExtensionToUse)
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

    for (auto* library : sub_libraries)
        presets.addArray (library->getPresets());
}

}  // namespace bav::plugin
