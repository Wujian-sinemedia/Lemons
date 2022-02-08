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

#include "lemons_musicology.h"


#include "util/StringFunctions.cpp"

#include "harmony/PitchClass.cpp"
#include "harmony/Pitch.cpp"
#include "harmony/Interval.cpp"

#include "harmony/scales/Scale.cpp"
#include "harmony/scales/Chromatic.cpp"
#include "harmony/scales/Octatonic.cpp"
#include "harmony/scales/WholeTone.cpp"
#include "harmony/scales/KeySignature.cpp"
#include "harmony/scales/Mode.cpp"

//#include "rhythm/TimeSignature.cpp"
//
//#include "harmony/Chord.cpp"

#if LEMONS_UNIT_TESTS
#	include "util/StringFunctions_tests.cpp"
#	include "harmony/PitchClass_tests.cpp"
#endif
