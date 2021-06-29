#pragma once

namespace bav
{

struct PluginPresetManager : PresetManagerBase
{
    PluginPresetManager (ParameterList& params, UndoManager* um = nullptr)
    : PresetManagerBase (params, um), list(params)
    {
    }
    
    PluginPresetManager (PluginState& state, UndoManager* um = nullptr)
    : PresetManagerBase (state, um), list(state.getParameters())
    {
    }
    
private:
    ParameterList& list;
    
    events::Listener l {getPresetLoadedBroadcaster(), [&]{ list.refreshAllDefaults(); }};
};

}
