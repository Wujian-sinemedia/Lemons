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


#include "lemons_plugin.h"


#include "parameters/utils/detail.cpp"
#include "parameters/utils/ParameterTraits.cpp"
#include "parameters/Parameter.cpp"
#include "parameters/types/ParameterTypes.cpp"
#include "parameters/types/Specializations.cpp"
#include "parameters/types/MeterParameter.cpp"
#include "parameters/types/MetaParameter.cpp"
#include "parameters/ModulationManager/LFO.cpp"
#include "parameters/ModulationManager/ModulationManager.cpp"

#include "util/serializing.cpp"

#include "processors/ProcessorAttributes.cpp"
#include "processors/BasicProcessor.cpp"
#include "processors/ProcessorBase.cpp"

#include "state/ParameterList.cpp"
#include "state/ProgramManager.cpp"
#include "state/PluginState.cpp"

#include "hosting/ProcessorHolder.cpp"
#include "hosting/ProcessorEngine.cpp"

#include "unit_tests/audio_processor_test.cpp"
#include "unit_tests/parameter_tests.cpp"
