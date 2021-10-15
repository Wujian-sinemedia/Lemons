#pragma once

/*-------------------------------------------------------------------------------------

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                 lemons_plugin_parameters
 vendor:             Ben Vining
 version:            0.0.1
 name:               lemons_plugin_parameters
 description:        Plugin parameter classes, and related building blocks
 dependencies:       lemons_oscillators

 END_JUCE_MODULE_DECLARATION

-------------------------------------------------------------------------------------*/


#include "parameters/Parameter.h"
#include "parameters/ParameterTypes.h"
#include "parameters/Specializations.h"

#include "parameters/utils/ParamUpdater.h"
#include "parameters/utils/ParameterHolder.h"

#include "parameters/ParameterList.h"

#include "parameters/MetaParameter.h"

#include "parameters/utils/ParameterProcessor.h"

#include "ModulationManager/LFO.h"
#include "ModulationManager/ModulationManager.h"
