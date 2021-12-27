/*
 ======================================================================================
 
 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 
 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 
 ======================================================================================
 */


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
 dependencies:       lemons_dsp lemons_musicology

 END_JUCE_MODULE_DECLARATION

-------------------------------------------------------------------------------------*/


#if LEMONS_UNIT_TESTS
#  include <lemons_unit_testing/lemons_unit_testing.h>
#endif


#include "parameters/Parameter.h"
#include "parameters/ParameterTypes.h"
#include "parameters/Specializations.h"
#include "parameters/MeterParameter.h"
//#include "parameters/MetaParameter.h"

#include "parameters/utils/ParamUpdater.h"

// modulation manager

#include "processors/ProcessorAttributes.h"
#include "processors/BasicProcessor.h"

#include "state/ProgramManager.h"
#include "state/PluginState.h"

#include "processors/ProcessorBase.h"

#include "editors/Editors.h"

#include "processors/Processors.h"

#include "hosting/ProcessorHolder.h"
#include "hosting/ProcessorEngine.h"

