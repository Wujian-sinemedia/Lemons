#pragma once

#if 0

BEGIN_JUCE_MODULE_DECLARATION

ID:                 bv_dsp
vendor:             Ben Vining
version:            0.0.1
name:               bv_dsp
description:        DSP/audio utilities and effects
dependencies:       juce_audio_utils juce_dsp bv_midi

END_JUCE_MODULE_DECLARATION

#endif

#include "Oscillators/oscillators.h"

#include "FIFOs/AudioFIFO.h"
#include "FIFOs/AudioAndMidiFIFO.h"

#include "engines/AudioEngine.h"
#include "engines/LatencyEngine.h"
#include "MidiChoppingProcessor/MidiChoppingProcessor.h"

#include "PitchDetector/pitch-detector.h"
#include "PSOLA/analysis/psola_analyzer.h"
#include "PSOLA/resynthesis/psola_shifter.h"

// FX
#include "FX/ReorderableFxChain.h"
#include "FX/dynamics/SmoothedGain.h"
#include "FX/dynamics/NoiseGate.h"
#include "FX/misc/DeEsser.h"
#include "FX/dynamics/Compressor.h"
#include "FX/misc/Reverb.h"
#include "FX/dynamics/Limiter.h"
#include "FX/stereo_image/MonoStereoConverter.h"
#include "FX/stereo_image/panning/PannerBase.h"
#include "FX/stereo_image/panning/MonoToStereoPanner.h"
#include "FX/stereo_image/panning/StereoPanner.h"
#include "FX/time/Delay.h"
#include "FX/misc/DryWet.h"

// synth
#include "synth/SynthVoice/SynthVoice.h"
#include "synth/Synth.h"
#include "synth/sine_synth.h"
