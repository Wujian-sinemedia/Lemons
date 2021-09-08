#pragma once

namespace bav::plugin
{
class DatabasePresetsManager
{
public:
    DatabasePresetsManager();

    const juce::Array< Preset >& getPresets();

private:
    juce::Array< Preset > presets;
};

}  // namespace bav::plugin
