#pragma once

/*-------------------------------------------------------------------------------------

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                 lemons_plugin
 vendor:             Lemons
 version:            0.0.1
 name:               lemons_plugin
 description:        Abstractions and building blocks for plugins
 website:            http://benthevining.github.io/Lemons/
 license:            GPL-3.0
 minimumCppStandard: 17
 dependencies:       lemons_dsp

 END_JUCE_MODULE_DECLARATION

-------------------------------------------------------------------------------------*/


#include "parameters/Parameter.h"
#include "parameters/ParameterTypes.h"
#include "parameters/Specializations.h"
#include "parameters/MetaParameter.h"

#include "parameters/utils/ParamUpdater.h"
#include "parameters/utils/ParameterProcessor.h"

// modulation manager

#include "state/state.h"

#include "processors/ProcessorBase.h"

#include "EditorBase/PluginGUIBase.h"
#include "EditorBase/EditorBase.h"

#include "processors/Processors.h"
