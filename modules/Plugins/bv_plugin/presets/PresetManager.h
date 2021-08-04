#pragma once

namespace bav::plugin
{
class PresetManager
{
public:
    PresetManager (StateBase&    stateToUse,
                   const String& companyNameToUse,
                   const String& productNameToUse,
                   const String& presetFileExtensionToUse = ".xml",
                   UndoManager*  um                       = nullptr);

    virtual ~PresetManager() = default;

    File presetsFolder();

    bool loadPreset (const String& presetName);
    void savePreset (const String& presetName);
    void deletePreset (const String& presetName);
    bool renamePreset (const String& previousName, const String& newName);

    void rescanPresetsFolder();

    const juce::StringArray& presetNames();

    events::Broadcaster& getPresetsChangedBroadcaster();
    events::Broadcaster& getPresetLoadedBroadcaster();

private:
    File presetNameToFilePath (const String& presetName);

    const String companyName;
    const String productName;
    const String presetFileExtension;

    UndoManager* undo;
    StateBase&   state;

    juce::StringArray namesOfAvailablePresets;

    events::Broadcaster availablePresetsChanged;
    events::Broadcaster presetLoaded;
};

}  // namespace bav::plugin
