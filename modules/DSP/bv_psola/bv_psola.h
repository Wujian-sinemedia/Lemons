#pragma once

#if 0

BEGIN_JUCE_MODULE_DECLARATION

ID:                 bv_psola
vendor:             Ben Vining
version:            0.0.1
name:               bv_psola
description:        DSP/audio utilities and effects
dependencies:       bv_core

END_JUCE_MODULE_DECLARATION

#endif

#include "analysis/PitchDetector/pitch-detector.h"
#include "analysis/psola_analyzer.h"
#include "resynthesis/psola_shifter.h"
