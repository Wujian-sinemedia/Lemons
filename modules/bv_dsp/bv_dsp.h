/*******************************************************************************
 BEGIN_JUCE_MODULE_DECLARATION
 ID:                 bv_dsp
 vendor:             Ben Vining
 version:            0.0.1
 name:               bv_dsp
 description:        DSP and audio utilities and effects
 dependencies:       juce_audio_utils juce_dsp bv_SharedCode
 END_JUCE_MODULE_DECLARATION
 *******************************************************************************/

#pragma once

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
#include "dsp/FX/dynamics/SmoothedGain.h"
#include "dsp/FX/dynamics/NoiseGate.h"
#include "dsp/FX/DeEsser.h"
#include "dsp/FX/Distortion.h"
#include "dsp/FX/dynamics/Compressor.h"
#include "dsp/FX/Reverb.h"
#include "dsp/FX/dynamics/Limiter.h"
#include "dsp/FX/stereo_image/MonoStereoConverter.h"
#include "dsp/FX/stereo_image/panning/PannerBase.h"
#include "dsp/FX/stereo_image/panning/MonoToStereoPanner.h"
#include "dsp/FX/stereo_image/panning/StereoPanner.h"
#include "dsp/FX/stereo_image/StereoWidener.h"
#include "dsp/FX/time/BeatRepeat.h"
#include "dsp/FX/time/Delay.h"
#include "dsp/FX/time/Freezer.h"
#include "dsp/FX/time/Looper.h"
#include "dsp/FX/pitch/PitchShifter.h"
#include "dsp/FX/pitch/PitchCorrector.h"
