#pragma once

/*-------------------------------------------------------------------------------------

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                 lemons_plugin_parameters
 vendor:             Ben Vining
 version:            0.0.1
 name:               lemons_plugin_parameters
 description:        Plugin parameter classes, done right
 dependencies:       lemons_oscillators

 END_JUCE_MODULE_DECLARATION

 -------------------------------------------------------------------------------------*/


#include "parameters/Parameter.h"
#include "parameters/ParameterTypes.h"
#include "parameters/Specializations.h"

#include "utils/ParamUpdater.h"

#include "RealtimeStateObject/RealtimeStateObject.h"

#include "ParameterList/ParameterList.h"

#include "processor/ParameterProcessor.h"

#include "ModulationManager/ModulationManager.h"
