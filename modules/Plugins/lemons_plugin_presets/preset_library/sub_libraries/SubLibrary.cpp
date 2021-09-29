
namespace lemons::plugin::presets
{
SubLibrary::SubLibrary (StateBase&    stateToUse,
                        const String& companyNameToUse,
                        const String& productNameToUse,
                        const String& presetFileExtensionToUse)
    : companyName (companyNameToUse), productName (productNameToUse), presetFileExtension (presetFileExtensionToUse), state (stateToUse)
{
}

const juce::Array< Preset >& SubLibrary::getPresets()
{
    refreshPresetList();
    return presets;
}

}  // namespace lemons::plugin::presets
