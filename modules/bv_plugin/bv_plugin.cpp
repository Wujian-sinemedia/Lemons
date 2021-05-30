
#include "bv_plugin.h"

#include "ProcessorBase/ProcessorBase.cpp"

#include "EditorBase/EditorBase.cpp"

#include "parameters/Parameter.cpp"
#include "parameters/types/ParameterTypes.cpp"
#include "parameters/specializations/ParameterSpecializations.cpp"
#include "parameters/ParameterList/ParameterList.cpp"

#include "state/state.cpp"

#include "transport/PluginTransport.cpp"

#if JucePlugin_Build_Standalone && JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP
#    include "StandaloneWrapper/StandaloneWrapper.cpp"
#endif
