#pragma once

/*-------------------------------------------------------------------------------------

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                 lemons_unit_testing
 vendor:             Lemons
 version:            0.0.1
 name:               lemons_unit_testing
 description:        Helpers for unit testing
 website:            http://benthevining.github.io/Lemons/
 license:            GPL-3.0
 minimumCppStandard: 17
 dependencies:       lemons_plugin_hosting lemons_musicology

 END_JUCE_MODULE_DECLARATION

-------------------------------------------------------------------------------------*/


//==============================================================================
/** Config: LEMONS_UNIT_TESTS

 Set this to 1 to build the Lemons unit tests.
 The Lemons repo's cmake scripts set this automatically for you; if that's how you added the Lemons package, then you don't need to worry about setting this manually.
 Inclusion of the lemons_core header also sets this to 1.
 */
#ifndef LEMONS_UNIT_TESTS
#  define LEMONS_UNIT_TESTS 1
#endif

#if ! LEMONS_UNIT_TESTS
#  error "Unit tests are disabled!"
#endif


//==============================================================================
/** Config: LEMONS_GUI_UNIT_TESTS

 Set this to 1 to include GUI unit tests.
 */
#ifndef LEMONS_GUI_UNIT_TESTS
#  define LEMONS_GUI_UNIT_TESTS 0
#endif


//==============================================================================
/** Config: LEMONS_BINARIES_UNIT_TESTS

 Set this to 1 to include unit tests involving loading data from a Juce BinaryData target.
 */
#ifndef LEMONS_BINARIES_UNIT_TESTS
#  if __has_include("BinaryData.h")
#	define LEMONS_BINARIES_UNIT_TESTS 1
#  else
#	define LEMONS_BINARIES_UNIT_TESTS 0
#  endif
#endif

#if LEMONS_BINARIES_UNIT_TESTS
#  if ! __has_include("BinaryData.h")
#	error "Cannot find the BinaryData header!"
#  endif
#endif

/*=======================================================================*/


#include "util/dsp_helpers.h"
#include "util/graphics_helpers.h"

#include "tests/test.h"
#include "tests/audio_processor_test.h"

#include "runner/runner.h"
