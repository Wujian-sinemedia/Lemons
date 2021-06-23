
#include "bv_plugin.h"


#include "processors/BasicProcessor.cpp"

#include "plugin_parameters/parameters/Parameter.cpp"
#include "plugin_parameters/parameters/types/ParameterTypes.cpp"
#include "plugin_parameters/parameters/specializations/ParameterSpecializations.cpp"
#include "plugin_parameters/StringProperty/StringProperty.cpp"
#include "plugin_parameters/ParameterList/ParameterList.cpp"
#include "plugin_parameters/processor/ParameterProcessor.cpp"

#include "processors/ProcessorBase.cpp"

#include "EditorBase/EditorBase.cpp"

#include "plugin_parameters/state/state.cpp"

#include "transport/PluginTransport.cpp"

#if JucePlugin_Build_Standalone && JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP
#    include "StandaloneWrapper/Window/StandaloneFilterWindow.cpp"
#    include "StandaloneWrapper/App/StandaloneFilterApp.cpp"
#endif
