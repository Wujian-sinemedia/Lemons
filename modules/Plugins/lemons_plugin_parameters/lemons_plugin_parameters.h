#pragma once

/*-------------------------------------------------------------------------------------

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                 lemons_plugin_parameters
 vendor:             Lemons
 version:            0.0.1
 name:               lemons_plugin_parameters
 description:        Plugin parameter classes, and related building blocks
 website:            http://benthevining.github.io/Lemons/
 license:            GPL-3.0
 minimumCppStandard: 17
 dependencies:       lemons_dsp

 END_JUCE_MODULE_DECLARATION

-------------------------------------------------------------------------------------*/


#include "aliases.h"

#include "parameters/Parameter.h"
#include "parameters/ParameterTypes.h"
#include "parameters/Specializations.h"

#include "parameters/utils/ParamUpdater.h"

#include "parameters/MetaParameter.h"

#include "parameters/utils/ParameterProcessor.h"

#include "ModulationManager/LFO.h"
#include "ModulationManager/ModulationManager.h"
