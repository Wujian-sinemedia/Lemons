#pragma once

/*******************************************************************************
 BEGIN_JUCE_MODULE_DECLARATION
 ID:                 bv_plugin
 vendor:             Ben Vining
 version:            0.0.1
 name:               bv_plugin
 description:        General plugin utilities & tools
 dependencies:       bv_core
 END_JUCE_MODULE_DECLARATION
 *******************************************************************************/

#include "bv_core/bv_core.h"

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

#include "StandaloneWrapper/StandaloneWrapper.h"
