#pragma once

/*-------------------------------------------------------------------------------------

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                 bv_plugin
 vendor:             Ben Vining
 version:            0.0.1
 name:               bv_plugin
 description:        General plugin utilities & tools
 dependencies:       bv_plugin_parameters bv_mobile_utils

 END_JUCE_MODULE_DECLARATION

-------------------------------------------------------------------------------------*/


#include "state/StateToggler.h"
#include "state/state.h"
#include "state/PluginState.h"

#include "processors/ProcessorBase.h"

#include "EditorBase/PluginGUIBase.h"
#include "EditorBase/EditorBase.h"

#include "processors/Processors.h"
