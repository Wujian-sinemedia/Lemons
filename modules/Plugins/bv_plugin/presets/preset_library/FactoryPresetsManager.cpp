namespace bav::plugin
{
FactoryPresetsManager::FactoryPresetsManager()
{
}

const juce::Array< Preset >& FactoryPresetsManager::getPresets()
{
    return presets;
}

}  // namespace bav::plugin
