
/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
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
 dependencies:       lemons_dsp lemons_musicology juce_audio_utils

 END_JUCE_MODULE_DECLARATION

-------------------------------------------------------------------------------------*/


#include "parameters/utils/detail.h"

#include "metadata/ParameterTraits.h"

#include "hosting/ProcessorHolder.h"

#include "unit_tests/audio_processor_test.h"
#include "processors/BasicProcessor.h"

#include "parameters/Parameter.h"

#include "parameters/types/ParameterTypes.h"
#include "parameters/types/Specializations.h"
#include "parameters/types/MeterParameter.h"

#include "state/ParameterList.h"
#include "state/ProgramManager.h"
#include "state/PluginState.h"

#include "parameters/types/MetaParameter.h"

#include "parameters/ModulationManager/LFO.h"
#include "parameters/ModulationManager/ModulationManager.h"

#include "hosting/ProcessorEngine.h"

#include "metadata/EditorAttributes.h"
#include "metadata/ProcessorAttributes.h"

#include "processors/ProcessorBase.h"

#include "metadata/PluginMetadata.h"

#include "editors/Editors.h"

#include "processors/Processors.h"

#include "hosting/PluginScanning.h"
