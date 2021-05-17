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


// midi
#include "midi/MidiFIFO.h"
#include "midi/MidiUtilities.h"
#include "midi/PitchbendTracker.h"
#include "midi/PitchConverter.h"
#include "midi/Chord.h"


// dsp
#include "dsp/Oscillators/oscillators.h"
#include "dsp/Oscillators/LFO/LFO.h"
#include "dsp/FIFOs/AudioFIFO.h"
#include "dsp/FIFOs/AudioAndMidiFIFO.h"
#include "dsp/FIFOWrappedEngine/FIFOWrappedEngine.h"
#include "dsp/FFT/bv_FFT.h"
#include "dsp/PitchDetector/pitch-detector.h"
#include "dsp/PSOLA/analysis/psola_analyzer.h"
#include "dsp/PSOLA/resynthesis/psola_shifter.h"

// dsp FX
#include "dsp/FX/ReorderableFxChain.h"
#include "dsp/FX/dynamics/NoiseGate.h"
#include "dsp/FX/DeEsser.h"
#include "dsp/FX/Distortion.h"
#include "dsp/FX/dynamics/Compressor.h"
#include "dsp/FX/Reverb.h"
#include "dsp/FX/SmoothedGain.h"
#include "dsp/FX/dynamics/Limiter.h"
#include "dsp/FX/stereo_image/MonoStereoConverter.h"
#include "dsp/FX/stereo_image/MonoToStereoPanner.h"
#include "dsp/FX/stereo_image/StereoPanner.h"
#include "dsp/FX/stereo_image/StereoWidener.h"
#include "dsp/FX/time/BeatRepeat.h"
#include "dsp/FX/time/Delay.h"
#include "dsp/FX/time/Freezer.h"
#include "dsp/FX/time/Looper.h"
#include "dsp/FX/pitch/PitchShifter.h"
#include "dsp/FX/pitch/PitchCorrector.h"


// parameters
#include "parameters/helpers/ParameterValueConversionLambdas.h"
#include "parameters/Parameter_Base.h"
#include "parameters/Parameter_ProcessorOwned.h"
#include "parameters/Parameter_SelfOwned.h"
#include "parameters/Parameter_List.h"
#include "parameters/helpers/ParameterHelpers.h"
#include "parameters/mappings/MidiCC_Mapping.h"
#include "parameters/mappings/LFO_Mapping.h"


// value trees
#include "valuetrees/property_nodes/nodes.h"
#include "valuetrees/property_nodes/propertyNodeGroup.h"
#include "valuetrees/Utils.h"
#include "valuetrees/json_converter.h"
#include "valuetrees/attachments/ParameterAttachments.h"
#include "valuetrees/attachments/FreestandingParameterAttachments.h"
#include "valuetrees/attachments/property-attachments/PropertyToValueTreeAttachments.h"
#include "valuetrees/attachments/property-attachments/ValueTreeToPropertyAttachments.h"
#include "valuetrees/attachments/property-attachments/PropertyAttachmentUtils.h"


// files
#include "FileUtilities.h"


// localization
#include "localization/localization.h"


// gui
#include "gui/icons.h"
#include "gui/gui.h"
#include "gui/Spline.h"
#include "gui/components/ParameterOwningComponent.h"
#include "gui/components/LevelMeters.h"
#include "gui/components/SynthMidiKeyboardComponent/SynthMidiKeyboardComponent.h"
#include "gui/components/SynthStaffDisplayComponent/SynthStaffDisplayComponent.h"


// network
#include "network/AsyncDownload.h"
#include "network/DownloadManager.h"


//==============================================================================
// motion (mobile only, experimental)

//#if JUCE_IOS
//  #include "motion/iOS/iosMotion.h"
//#elif JUCE_ANDROID
//  #include "motion/Android/androidMotion.h"
//#endif

//==============================================================================


#ifdef __clang__
  #pragma clang diagnostic pop
#endif


#endif  /* ifndef __BV_SHARED_CODE_H__ */
