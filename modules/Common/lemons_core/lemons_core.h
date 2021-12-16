/*
 ======================================================================================
 
 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 
 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 
 ======================================================================================
 */


#pragma once

/*-------------------------------------------------------------------------------------

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                 lemons_core
 vendor:             Lemons
 version:            0.0.1
 name:               lemons_core
 description:        General utilities useful for developing plugins
 website:            http://benthevining.github.io/Lemons/
 license:            GPL-3.0
 minimumCppStandard: 17
 dependencies:       juce_audio_formats juce_audio_processors juce_gui_basics
 OSXFrameworks:      Accelerate
 iOSFrameworks:      Accelerate

 END_JUCE_MODULE_DECLARATION

 -------------------------------------------------------------------------------------*/


//==============================================================================
/** Config: LEMONS_HAS_BINARY_DATA

    Set this to 1 if your project has a JUCE binary data target.
    The Lemons repo's cmake scripts set this automatically for you; if that's how you added the Lemons package, then you don't need to worry about setting this manually.
 */
#ifndef LEMONS_HAS_BINARY_DATA
#  if __has_include("BinaryData.h")
#	define LEMONS_HAS_BINARY_DATA 1
#  else
#	define LEMONS_HAS_BINARY_DATA 0
#  endif
#endif

#if LEMONS_HAS_BINARY_DATA
#  if ! __has_include("BinaryData.h")
#	error "LEMONS_HAS_BINARY_DATA is on, but the BinaryData header cannot be found!"
#  endif

/** Config: LEMONS_BINARY_DATA_NAMESPACE
 
    Use this to specify the namespace your binary data assets are in. Defaults to 'BinaryData'.
 */
#ifndef LEMONS_BINARY_DATA_NAMESPACE
#define LEMONS_BINARY_DATA_NAMESPACE BinaryData
#endif

#endif

//==============================================================================
/** Config: LEMONS_USE_VDSP

    Set this to 1 to use Apple's vDSP library for vecops SIMD.
    vDSP ships with the OS on Mac and iOS, which is why it's the default on Apple platforms.
    The Lemons repo's cmake scripts set this automatically for you; if that's how you added the Lemons package, then you don't need to worry about setting this manually.
 */
#ifndef LEMONS_USE_VDSP
#  if JUCE_IOS || JUCE_MAC
#	define LEMONS_USE_VDSP 1
#  else
#	define LEMONS_USE_VDSP 0
#  endif
#endif


//==============================================================================
/** Config: LEMONS_UNIT_TESTS

    Set this to 1 to build the Lemons unit tests.
    The Lemons repo's cmake scripts set this automatically for you; if that's how you added the Lemons package, then you don't need to worry about setting this manually.
 */
#ifndef LEMONS_UNIT_TESTS
#  if __has_include("lemons_unit_testing/lemons_unit_testing.h")
#	define LEMONS_UNIT_TESTS 1
#  else
#	define LEMONS_UNIT_TESTS 0
#  endif
#endif

/*=======================================================================*/

#include <juce_data_structures/juce_data_structures.h>


namespace lemons
{

using juce::File;
using juce::String;
using juce::ValueTree;

}


#include "misc/Preprocessor.h"
#include "misc/Version.h"
#include "misc/Dimensions.h"
#include "misc/ConstructedArray.h"

#include "events/Timers.h"
#include "events/AutoLock.h"
#include "events/DeviceRotation.h"
#include "events/DeviceMotion.h"

#include "files/FileUtilities.h"
#include "files/AudioFile.h"

#include "serializing/serializing.h"
#include "serializing/VariantConversions.h"

#include "binary_data/data.h"

#include "localization/localization.h"

#include "math/mathHelpers.h"
#include "math/ValueSmoother.h"
#include "math/vecops.h"
#include "misc/ArgParser.h"
