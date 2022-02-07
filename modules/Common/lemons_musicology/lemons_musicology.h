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

#pragma once

/*-------------------------------------------------------------------------------------

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                 lemons_musicology
 vendor:             Lemons
 version:            0.0.1
 name:               lemons_musicology
 description:        Utility classes for describing musical information
 website:            http://benthevining.github.io/Lemons/
 license:            GPL-3.0
 minimumCppStandard: 17
 dependencies:       lemons_core

 END_JUCE_MODULE_DECLARATION

 -------------------------------------------------------------------------------------*/


#include "util/PitchUtils.h"
#include "util/StringFunctions.h"

#include "harmony/Pitch.h"
#include "harmony/Interval.h"
#include "harmony/Interval_impl.h"

#include "harmony/scales/Scale.h"
#include "harmony/scales/Chromatic.h"
#include "harmony/scales/WholeTone.h"
#include "harmony/scales/KeySignature.h"
#include "harmony/scales/KeySignature_impl.h"
#include "harmony/scales/Mode.h"

//#include "rhythm/TimeSignature.h"
//
//#include "harmony/Chord.h"
