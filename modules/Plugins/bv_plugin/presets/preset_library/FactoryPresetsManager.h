#pragma once

namespace bav::plugin
{
class FactoryPresetsManager
/*
 - should write the factory presets to the filesystem if they're missing
 */
{
public:
    FactoryPresetsManager();

    const juce::Array< Preset >& getPresets();

private:
    juce::Array< Preset > presets;
};

}  // namespace bav::plugin
