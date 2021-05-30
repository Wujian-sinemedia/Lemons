/******************************************************************************
 BEGIN_JUCE_MODULE_DECLARATION
 ID:                 bv_dsp
 vendor:             Ben Vining
 version:            0.0.1
 name:               bv_dsp
 description:        DSP and audio utilities and effects
 dependencies:       juce_audio_utils juce_dsp bv_midi bv_mts_esp_client
 END_JUCE_MODULE_DECLARATION
 *******************************************************************************/

#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_dsp/juce_dsp.h>

#include <bv_midi/bv_midi.h>
#include <bv_mts_esp_client/bv_mts_esp_client.h>


#include "Oscillators/oscillators.h"
#include "Oscillators/LFO/LFO.h"
#include "FIFOs/AudioFIFO.h"
#include "FIFOs/AudioAndMidiFIFO.h"
#include "FIFOWrappedEngine/FIFOWrappedEngine.h"
#include "PitchDetector/pitch-detector.h"
#include "PSOLA/analysis/psola_analyzer.h"
#include "PSOLA/resynthesis/psola_shifter.h"

// FX
#include "FX/ReorderableFxChain.h"
#include "FX/dynamics/SmoothedGain.h"
#include "FX/dynamics/NoiseGate.h"
#include "FX/misc/DeEsser.h"
#include "FX/misc/Distortion.h"
#include "FX/dynamics/Compressor.h"
#include "FX/misc/Reverb.h"
#include "FX/misc/EQ.h"
#include "FX/dynamics/Limiter.h"
#include "FX/stereo_image/MonoStereoConverter.h"
#include "FX/stereo_image/panning/PannerBase.h"
#include "FX/stereo_image/panning/MonoToStereoPanner.h"
#include "FX/stereo_image/panning/StereoPanner.h"
#include "FX/stereo_image/StereoWidener.h"
#include "FX/time/BeatRepeat.h"
#include "FX/time/Delay.h"
#include "FX/time/Freezer.h"
#include "FX/time/Looper.h"
#include "FX/pitch/PitchShifter.h"
#include "FX/pitch/PitchCorrector.h"

// synth
#include "synth/SynthVoice/SynthVoice.h"
#include "synth/Synth.h"
#include "synth/sine_synth.h"
