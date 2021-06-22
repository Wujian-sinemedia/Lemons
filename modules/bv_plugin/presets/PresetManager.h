#pragma once

namespace bav
{
class PresetManagerBase
{
public:
    PresetManagerBase (StateBase& stateToUse, UndoManager* um = nullptr);
    PresetManagerBase (ParameterList& listToUse, UndoManager* um = nullptr);

    virtual ~PresetManagerBase() = default;

    virtual std::string getCompanyName()         = 0;
    virtual std::string getProductName()         = 0;
    virtual std::string getPresetFileExtension() = 0;

    juce::File presetsFolder();

    bool loadPreset (const String& presetName);
    void savePreset (const String& presetName);
    void deletePreset (const String& presetName);
    bool renamePreset (const String& previousName, const String& newName);

    void rescanPresetsFolder();

    const juce::StringArray& presetNames();

    events::Broadcaster& getBroadcaster() { return broadcaster; }

private:
    juce::File presetNameToFilePath (const String& presetName);
    
    UndoManager* undo;

    StateBase*     state;
    ParameterList* list;

    juce::StringArray   namesOfAvailablePresets;
    events::Broadcaster broadcaster;
};

}  // namespace bav
