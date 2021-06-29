#pragma once

#if 0

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                 bv_plugin
 vendor:             Ben Vining
 version:            0.0.1
 name:               bv_plugin
 description:        General plugin utilities & tools
 dependencies:       bv_dsp bv_mobile_utils

 END_JUCE_MODULE_DECLARATION
 
#endif


//==============================================================================
/** Config: BV_USE_ABLETON_LINK
 
    Set this to 1 if your project is using the Ableton Link library.
    If this is 1, the transport class will sync with Ableton Link.
    If this is 0, only the host transport interface and the built-in fallback implementations will be used.
 */
#ifndef BV_USE_ABLETON_LINK
#    define BV_USE_ABLETON_LINK 0
#endif



#include <bv_dsp/bv_dsp.h>
#include <bv_mobile_utils/bv_mobile_utils.h>

#include "transport/PluginTransport.h"

#include "processors/BasicProcessor.h"

#include "plugin_parameters/parameters/helpers/ParameterValueConversionLambdas.h"
#include "plugin_parameters/parameters/Parameter.h"
#include "plugin_parameters/parameters/types/ParameterTypes.h"
#include "plugin_parameters/parameters/specializations/ParameterSpecializations.h"
#include "plugin_parameters/parameters/helpers/ParamUpdater.h"
#include "plugin_parameters/ParameterList/ParameterHolder.h"
#include "plugin_parameters/StringProperty/StringProperty.h"
#include "plugin_parameters/ParameterList/ParameterList.h"
#include "plugin_parameters/processor/ParameterProcessor.h"

#include "plugin_state/state.h"
#include "plugin_state/PluginUndo.h"
#include "plugin_state/PluginPresetManager.h"

#include "processors/ProcessorBase.h"

#include "EditorBase/EditorBase.h"

#include "processors/ProcessorWithEditor.h"


#ifndef JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP
#    define JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP 0
#endif

#ifndef JucePlugin_Build_Standalone
#    define JucePlugin_Build_Standalone 0
#endif

#if JucePlugin_Build_Standalone && JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP
#    include "StandaloneWrapper/Window/StandaloneFilterWindow.h"
#    include "StandaloneWrapper/App/StandaloneFilterApp.h"
#endif
