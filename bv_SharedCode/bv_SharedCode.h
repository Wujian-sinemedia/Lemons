/*******************************************************************************
 BEGIN_JUCE_MODULE_DECLARATION
 ID:                 bv_SharedCode
 vendor:             Ben Vining
 version:            0.0.1
 name:               Ben Vining's codebase
 description:        General utilities useful for developing plugins.
 dependencies:       juce_audio_utils juce_dsp juce_osc
 OSXFrameworks: Accelerate
 iOSFrameworks: CoreMotion
 END_JUCE_MODULE_DECLARATION
 *******************************************************************************/


#pragma once


#ifndef __BV_SHARED_CODE_H__
#define __BV_SHARED_CODE_H__


#ifndef BV_HAS_BINARY_DATA
  #define BV_HAS_BINARY_DATA 0
#endif


// JUCE dependencies
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_dsp/juce_dsp.h>
#include <juce_osc/juce_osc.h>


#include "core/System.h"


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


#include "core/misc.h"


#include "core/AlignedAllocate.h"
#include "core/DataHelpers.h"
#include "core/LockFreeFifo.h"

#include "math/mathHelpers.h"
#include "math/VectorOps.h"
#include "math/intOps.h"

#include "midi/MidiFIFO.h"
#include "midi/MidiUtilities.h"

#include "dsp/oscillators.h"
#include "dsp/AudioFIFO/AudioFIFO.h"
#include "dsp/Panner.h"
#include "dsp/FX/ReorderableFxChain.h"
#include "dsp/FX/NoiseGate.h"
#include "dsp/FX/Compressor.h"
#include "dsp/FX/Limiter.h"
#include "dsp/FX/DeEsser.h"
#include "dsp/FX/Reverb.h"
#include "dsp/FX/Delay.h"
#include "dsp/FIFOWrappedEngine/FIFOWrappedEngine.h"
#include "dsp/PitchDetector/pitch-detector.h"
#include "dsp/DummyAudioProcessor.h"

#include "dsp/FFT/bv_FFT.h"

#include "parameters/ParameterValueConversionLambdas.h"
#include "parameters/Parameters.h"
#include "parameters/MidiCC_Mapping.h"
#include "parameters/ParameterHelpers.h"

#include "valuetrees/property_nodes/nodes.h"
#include "valuetrees/property_nodes/propertyNodeGroup.h"
#include "valuetrees/property_nodes/NodeAttachments.h"
#include "valuetrees/Utils.h"
#include "valuetrees/ParameterAttachments.h"
#include "valuetrees/json_converter.h"

#include "gui/gui.h"
#include "gui/Spline.h"

#include "network/AsyncDownload.h"
#include "network/DownloadManager.h"

//==============================================================================

#if JUCE_IOS
  #include "core/motion/iosMotion.h"
#elif JUCE_ANDROID
  #include "core/motion/androidMotion.h"
#endif

//==============================================================================


#ifdef __clang__
  #pragma clang diagnostic pop
#endif


#endif  /* ifndef __BV_SHARED_CODE_H__ */
