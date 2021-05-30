#pragma once

/*******************************************************************************
 BEGIN_JUCE_MODULE_DECLARATION
 ID:                 bv_plugin
 vendor:             Ben Vining
 version:            0.0.1
 name:               bv_plugin
 description:        General plugin utilities & tools
 dependencies:       bv_core bv_mobile_utils
 END_JUCE_MODULE_DECLARATION
 *******************************************************************************/

#include "bv_core/bv_core.h"
#include "bv_mobile_utils/bv_mobile_utils.h"

#ifndef BV_USE_ABLETON_LINK
#    define BV_USE_ABLETON_LINK 0
#endif


#include "SystemInitializer.h"

#include "ProcessorBase/ProcessorBase.h"

#include "EditorBase/EditorBase.h"

#include "parameters/helpers/ParameterValueConversionLambdas.h"
#include "parameters/Parameter.h"
#include "parameters/types/ParameterTypes.h"
#include "parameters/specializations/ParameterSpecializations.h"
#include "parameters/mappings/MidiCC_Mapping.h"
#include "parameters/mappings/LFO_Mapping.h"
#include "parameters/ParameterList/ParameterHolder.h"
#include "parameters/ParameterList/ParameterList.h"

#include "state/state.h"

#include "transport/PluginTransport.h"

#if JucePlugin_Build_Standalone && JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP
#    include "StandaloneWrapper/StandaloneWrapper.h"
#endif
