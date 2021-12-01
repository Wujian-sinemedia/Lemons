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
 dependencies:       lemons_dsp

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


/*=======================================================================*/


#include "util/dsp_helpers.h"

#include "tests/test.h"
#include "tests/dsp_test.h"
#include "tests/audio_processor_test.h"

#include "runner/runner.h"
