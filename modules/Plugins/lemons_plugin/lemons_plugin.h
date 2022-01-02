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
 dependencies:       lemons_dsp lemons_musicology juce_audio_utils juce_audio_devices

 END_JUCE_MODULE_DECLARATION

-------------------------------------------------------------------------------------*/


#include "parameters/utils/detail.h"
#include "parameters/utils/ParameterTraits.h"

#include "parameters/Parameter.h"
#include "parameters/ParameterTypes.h"
#include "parameters/Specializations.h"
#include "parameters/MeterParameter.h"

#include "parameters/utils/ParamUpdater.h"

#include "state/ParameterList.h"
#include "state/ProgramManager.h"
#include "state/PluginState.h"

#include "parameters/MetaParameter.h"

// modulation manager

#include "hosting/ProcessorHolder.h"
#include "hosting/ProcessorEngine.h"

#include "unit_tests/audio_processor_test.h"
#include "unit_tests/parameter_tests.h"

#include "processors/ProcessorAttributes.h"
#include "processors/BasicProcessor.h"
#include "processors/ProcessorBase.h"

#include "util/Editors.h"
#include "util/serializing.h"

#include "processors/Processors.h"

