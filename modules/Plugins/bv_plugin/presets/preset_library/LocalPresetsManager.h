#pragma once

namespace bav::plugin
{
class LocalPresetsManager
/*
 TODO:
 add funcs to load, save, delete, and rename presets
 */
{
public:
    LocalPresetsManager (StateBase&    stateToUse,
                         const String& companyNameToUse,
                         const String& productNameToUse,
                         const String& presetFileExtensionToUse = ".xml",
                         UndoManager*  um                       = nullptr);

    virtual ~LocalPresetsManager() = default;

    File presetsFolder();

    const juce::Array< Preset >& getPresets();

private:
    void rescanPresetsFolder();

    const String companyName;
    const String productName;
    const String presetFileExtension;

    UndoManager* undo;
    StateBase&   state;

    juce::Array< Preset > presets;
};

}  // namespace bav::plugin
