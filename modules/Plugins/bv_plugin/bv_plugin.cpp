
#include "bv_plugin.h"

#include "state/state.cpp"

#include "processors/ProcessorBase.cpp"

#include "EditorBase/EditorBase.cpp"

#include "transport/PluginTransport.cpp"

#if JucePlugin_Build_Standalone && JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP
#    include "StandaloneWrapper/Window/StandaloneFilterWindow.cpp"
#    include "StandaloneWrapper/App/StandaloneFilterApp.cpp"
#endif
