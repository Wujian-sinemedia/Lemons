/*******************************************************************************
 BEGIN_JUCE_MODULE_DECLARATION
 ID:                 bv_SharedCode
 vendor:             Ben Vining
 version:            0.0.1
 name:               Ben Vining's codebase
 description:        General utilities useful for developing plugins.
 dependencies:       juce_audio_utils juce_osc
 OSXFrameworks: Accelerate
 END_JUCE_MODULE_DECLARATION
 *******************************************************************************/


#pragma once


#ifndef __BV_SHARED_CODE_H__
#define __BV_SHARED_CODE_H__


#ifndef BV_HAS_BINARY_DATA
  #define BV_HAS_BINARY_DATA 0
#endif


// juce dependencies
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_osc/juce_osc.h>


#include "core/PlatformDefs.h"


#if BV_USE_VDSP
  #include <Accelerate/Accelerate.h>
#elif BV_USE_IPP
  #include <ippversion.h>
  #include <ipps.h>
#elif BV_USE_NE10
  #include <NE10.h>
#elif BV_USE_MIPP
  #include <mipp.h>
#endif


#ifdef __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wunused-function"
#endif


#include "valuetrees/DefaultValueTreeIDs.h"


// core
#include "core/misc.h"
#include "core/AlignedAllocate.h"
#include "core/DataHelpers.h"
#include "core/LockFreeFifo.h"
#include "core/RealtimeSpinLock.h"
#include "core/ValueListeners.h"


// building blocks
#include "building_blocks/ApplicationBase.h"
#include "building_blocks/ProcessorBase.h"
#include "building_blocks/PluginEditorBase.h"


// async
#include "async/AsyncUtils.h"


// math
#include "math/mathHelpers.h"
#include "math/VectorOps.h"
#include "math/intOps.h"


// files
#include "files/FileUtilities.h"


// localization
#include "localization/localization.h"

// network
#include "network/AsyncDownload.h"
#include "network/DownloadManager.h"


#ifdef __clang__
  #pragma clang diagnostic pop
#endif


#endif  /* ifndef __BV_SHARED_CODE_H__ */
