/*******************************************************************************
 BEGIN_JUCE_MODULE_DECLARATION
 ID:                 bv_SharedCode
 vendor:             Ben Vining
 version:            0.0.1
 name:               Ben Vining's codebase
 description:        General utilities useful for developing plugins.
 dependencies:       juce_audio_utils bv_mobile_utils
 OSXFrameworks:      Accelerate
 iOSFrameworks:      Accelerate
 END_JUCE_MODULE_DECLARATION
 *******************************************************************************/


#pragma once


#ifndef BV_HAS_BINARY_DATA
#    define BV_HAS_BINARY_DATA 0
#endif


#include <juce_audio_utils/juce_audio_utils.h>

#include <bv_mobile_utils/bv_mobile_utils.h>


#include "preprocessor/PlatformDefs.h"
#include "preprocessor/PreprocessorUtils.h"


#ifdef __clang__
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wunused-function"
#endif


#if BV_USE_VDSP
#    include <Accelerate/Accelerate.h>
#elif BV_USE_IPP
#    include <ippversion.h>
#    include <ipps.h>
#elif BV_USE_NE10
#    include <NE10.h>
#elif BV_USE_MIPP
#    include <mipp.h>
#endif


// misc
#include "misc/StaticObjects.h"
#include "misc/misc.h"
#include "misc/AlignedAllocate.h"
#include "misc/LockFreeFifo.h"

#include "serializing/SerializableData.h"
#include "serializing/valuetree_json_converter.h"


// files
#include "files/FileUtilities.h"
#include "files/BinaryDataHelpers/BinaryDataHelpers.h"


// localization
#include "localization/localization.h"

#include "misc/SystemInitializer.h"


// async
#include "async/AsyncUtils.h"


#include "parameters/helpers/ParameterValueConversionLambdas.h"
#include "parameters/Parameter.h"
#include "parameters/types/ParameterTypes.h"
#include "parameters/specializations/ParameterSpecializations.h"
#include "parameters/mappings/MidiCC_Mapping.h"
#include "parameters/mappings/LFO_Mapping.h"
#include "parameters/ParameterList/ParameterHolder.h"
#include "parameters/ParameterList/ParameterList.h"


// math
#include "math/mathHelpers.h"
#include "math/vecops/vecops.h"
#include "math/intops/intOps.h"


// network
#include "network/AsyncDownload.h"
#include "network/DownloadManager.h"


#ifdef __clang__
#    pragma clang diagnostic pop
#endif
