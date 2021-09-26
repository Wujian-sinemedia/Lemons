#pragma once

/*-------------------------------------------------------------------------------------

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                 bv_plugin_parameters
 vendor:             Ben Vining
 version:            0.0.1
 name:               bv_plugin_parameters
 description:        Plugin parameter classes, done right
 dependencies:       bv_oscillators bv_serializing

 END_JUCE_MODULE_DECLARATION

 -------------------------------------------------------------------------------------*/


#include "parameters/helpers/ParameterValueConversionLambdas.h"
#include "parameters/Parameter.h"
#include "parameters/ParameterTypes.h"
#include "parameters/specializations/ParameterSpecializations.h"
#include "parameters/helpers/ParamUpdater.h"

#include "RealtimeStateObject/RealtimeStateObject.h"

#include "ParameterList/ParameterList.h"

#include "processor/ParameterProcessor.h"

#include "ModulationManager/ModulationManager.h"
#include "ModulationManager/processor.h"
