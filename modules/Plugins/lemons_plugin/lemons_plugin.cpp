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


#include "parameters/Parameter.cpp"
#include "parameters/ParameterTypes.cpp"
#include "parameters/Specializations.cpp"
//#include "parameters/MetaParameter.cpp"

#include "parameters/utils/ParamUpdater.cpp"
//#include "parameters/utils/ParameterProcessor.cpp"

#include "processors/ProcessorAttributes.cpp"
#include "processors/BasicProcessor.cpp"
#include "processors/PluginState.cpp"
#include "processors/ProcessorBase.cpp"

#include "hosting/ProcessorHolder.cpp"
#include "hosting/ProcessorEngine.cpp"