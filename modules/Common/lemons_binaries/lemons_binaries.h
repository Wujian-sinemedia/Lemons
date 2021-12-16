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

 ID:                 lemons_binaries
 vendor:             Lemons
 version:            0.0.1
 name:               lemons_binaries
 description:        Utilities for working with embedded binary data
 website:            http://benthevining.github.io/Lemons/
 license:            GPL-3.0
 minimumCppStandard: 17
 dependencies:       lemons_core lemons_midi lemons_dsp juce_audio_formats juce_graphics

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

/*=======================================================================*/


#include "audio_files/AudioFile.h"

#include "binary_data/data.h"

#include "localization/localization.h"

