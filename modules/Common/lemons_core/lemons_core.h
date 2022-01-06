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
 description:        General core utilities
 website:            http://benthevining.github.io/Lemons/
 license:            GPL-3.0
 minimumCppStandard: 17
 dependencies:       juce_data_structures juce_core juce_cryptography juce_graphics

 END_JUCE_MODULE_DECLARATION

 -------------------------------------------------------------------------------------*/


//==============================================================================
/** Config: LEMONS_UNIT_TESTS

    Set this to 1 to build the Lemons unit tests.
    The Lemons repo's cmake scripts set this automatically for you; if that's how you added the Lemons package, then you don't need to worry about setting this manually.
 */
#ifndef LEMONS_UNIT_TESTS
#  define LEMONS_UNIT_TESTS 0
#endif


//==============================================================================
/** Config: LEMONS_GUI_UNIT_TESTS

 Set this to 1 to include GUI unit tests.
 */
#ifndef LEMONS_GUI_UNIT_TESTS
#  define LEMONS_GUI_UNIT_TESTS 0
#endif


//==============================================================================
/** Config: LEMONS_HAS_BINARY_DATA

    Set this to 1 if your project has a juce BinaryData target.
    The Lemons repo's cmake scripts set this automatically for you; if that's how you added the Lemons package, then you don't need to worry about setting this manually.
 */
#ifndef LEMONS_HAS_BINARY_DATA
#  if __has_include("BinaryData/BinaryData.h")
#    define LEMONS_HAS_BINARY_DATA 1
#  else 
#    define LEMONS_HAS_BINARY_DATA 0
#  endif
#endif

/*=======================================================================*/


#include "misc/Preprocessor.h"

#include "misc/Version.h"
#include "misc/Dimensions.h"
#include "misc/ConstructedArray.h"
#include "misc/Timers.h"
#include "misc/Ranges.h"
#include "misc/rsa.h"

#include "unit_tests/test.h"
#include "unit_tests/runner.h"

#include "files/FileUtilities.h"

#include "binary_data/data.h"
#include "binary_data/translations.h"

#include "serializing/serializing.h"
#include "serializing/VariantConversions.h"

#include "localization/languages.h"
#include "localization/TranslationFiles.h"

#include "math/mathHelpers.h"
#include "misc/ArgParser.h"
