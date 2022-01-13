#pragma once

/*-------------------------------------------------------------------------------------

BEGIN_JUCE_MODULE_DECLARATION

ID:                 lemons_psola
vendor:             Lemons
version:            0.0.1
name:               lemons_psola
description:        Classes for PSOLA pitch shifting of monophonic audio
website:            http://benthevining.github.io/Lemons/
license:            GPL-3.0
minimumCppStandard: 17
dependencies:       lemons_dsp

END_JUCE_MODULE_DECLARATION

 -------------------------------------------------------------------------------------*/


#include "analysis/PeakFinder.h"
#include "analysis/PitchDetector.h"
#include "analysis/analyzer.h"

#include "resynthesis/shifter.h"
#include "resynthesis/RatioShifter.h"
#include "resynthesis/IntervalShifter.h"
