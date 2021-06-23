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

#include "FIFOs/AudioFIFO.h"
#include "FIFOs/AudioAndMidiFIFO.h"

#include "engines/AudioEngine.h"
#include "engines/LatencyEngine.h"

#include "MidiChoppingProcessor/MidiChoppingProcessor.h"

#include "Oscillators/oscillators.h"
#include "Oscillators/choosable/ChoosableOscillator.h"

#include "PitchDetector/pitch-detector.h"
#include "PSOLA/analysis/psola_analyzer.h"
#include "PSOLA/resynthesis/psola_shifter.h"

#include "filters/Filter.h"
