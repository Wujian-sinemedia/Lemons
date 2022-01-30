/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

#include "lemons_psola.h"


#include "analysis/PeakFinder.cpp"
#include "analysis/PitchDetector.cpp"
#include "analysis/analyzer.cpp"

#include "resynthesis/IntervalShifter.cpp"
#include "resynthesis/MidiShifter.cpp"
#include "resynthesis/RatioShifter.cpp"
#include "resynthesis/shifter.cpp"

#if LEMONS_UNIT_TESTS
#	include "analysis/PeakFinder_tests.cpp"
#	include "analysis/PitchDetector_tests.cpp"
#	include "resynthesis/IntervalShifter_tests.cpp"
#	include "resynthesis/shifter_tests.cpp"
#endif
