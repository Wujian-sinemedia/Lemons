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



#include "bv_dsp/bv_dsp.h"
#include "bv_mobile_utils/bv_mobile_utils.h"

#include "parameters/helpers/ParameterValueConversionLambdas.h"
#include "parameters/Parameter.h"
#include "parameters/types/ParameterTypes.h"
#include "parameters/specializations/ParameterSpecializations.h"
#include "parameters/ParameterList/ParameterHolder.h"

#include "transport/PluginTransport.h"

#include "ProcessorBase/BasicProcessor.h"

#include "parameters/ParameterList/ParameterList.h"
#include "parameters/processor/ParameterProcessor.h"

#include "ProcessorBase/ProcessorBase.h"

#include "EditorBase/EditorBase.h"

#include "state/state.h"
#include "state/PluginUndo.h"
#include "presets/PresetManager.h"


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
