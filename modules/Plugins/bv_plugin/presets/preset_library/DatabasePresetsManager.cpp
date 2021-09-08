namespace bav::plugin
{
DatabasePresetsManager::DatabasePresetsManager()
{
}

const juce::Array< Preset >& DatabasePresetsManager::getPresets()
{
    return presets;
}

}  // namespace bav::plugin
