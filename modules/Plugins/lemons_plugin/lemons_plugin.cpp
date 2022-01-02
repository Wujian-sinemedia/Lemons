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
#include "parameters/ParameterTypes.cpp"
#include "parameters/Specializations.cpp"
#include "parameters/MeterParameter.cpp"
#include "parameters/MetaParameter.cpp"

#include "parameters/utils/ParamUpdater.cpp"

#include "util/serializing.cpp"

#include "processors/ProcessorAttributes.cpp"
#include "processors/BasicProcessor.cpp"

#include "state/ParameterList.cpp"
#include "state/ProgramManager.cpp"
#include "state/PluginState.cpp"

#include "processors/ProcessorBase.cpp"

#include "hosting/ProcessorHolder.cpp"
#include "hosting/ProcessorEngine.cpp"

#include "unit_tests/audio_processor_test.cpp"
