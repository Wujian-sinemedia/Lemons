#pragma once

namespace bav
{

struct PluginPresetManager : PresetManagerBase
{
    PluginPresetManager (ParameterList& params, UndoManager* um = nullptr)
    : PresetManagerBase (params, um)
    {
        list = &params;
        state = nullptr;
    }
    
    PluginPresetManager (StateBase& stateToUse, UndoManager* um = nullptr)
    : PresetManagerBase (stateToUse, um)
    {
        state = &stateToUse;
        list = nullptr;
    }
    
private:
    void presetLoaded()
    {
        if (list != nullptr)
            list->refreshAllDefaults();
        else if (state != nullptr)
            state->refreshAllDefaults();
    }
    
    ParameterList* list;
    StateBase*     state;
    
    events::Listener l {getPresetLoadedBroadcaster(), [&]{ presetLoaded(); }};
};

}
