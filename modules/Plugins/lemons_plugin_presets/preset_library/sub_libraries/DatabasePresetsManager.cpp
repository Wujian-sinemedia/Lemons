namespace lemons::plugin::presets
{
DatabasePresetsManager::DatabasePresetsManager (StateBase&    stateToUse,
                                                const String& companyNameToUse,
                                                const String& productNameToUse,
                                                const String& presetFileExtensionToUse)
    : SubLibrary (stateToUse, companyNameToUse, productNameToUse, presetFileExtensionToUse)
{
    refreshPresetList();
}

void DatabasePresetsManager::refreshPresetList()
{
}

}  // namespace bav::plugin::presets
