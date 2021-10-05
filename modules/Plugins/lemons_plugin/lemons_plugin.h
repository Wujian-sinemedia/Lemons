#pragma once

/*-------------------------------------------------------------------------------------

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                 lemons_plugin
 vendor:             Ben Vining
 version:            0.0.1
 name:               lemons_plugin
 description:        Abstractions and building blocks for plugins
 dependencies:       lemons_oscillators lemons_mobile_utils

 END_JUCE_MODULE_DECLARATION

-------------------------------------------------------------------------------------*/


#include "parameters/Parameter.h"
#include "parameters/ParameterTypes.h"
#include "parameters/Specializations.h"

#include "parameters/utils/ParamUpdater.h"
#include "parameters/utils/ParameterHolder.h"

#include "parameters/ParameterList.h"

#include "parameters/utils/ParameterProcessor.h"

#include "ModulationManager/ModulationManager.h"

#include "state/state.h"
#include "state/PluginState.h"

#include "processors/ProcessorBase.h"

#include "EditorBase/PluginGUIBase.h"
#include "EditorBase/EditorBase.h"

#include "processors/Processors.h"
