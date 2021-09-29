#pragma once

/*-------------------------------------------------------------------------------------

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                lemons_core
 vendor:            Ben Vining
 version:           0.0.1
 name:              lemons_core
 description:       General utilities useful for developing plugins.
 dependencies:      juce_audio_utils
 OSXFrameworks:     Accelerate
 iOSFrameworks:     Accelerate

 END_JUCE_MODULE_DECLARATION

 -------------------------------------------------------------------------------------*/


//==============================================================================
/** Config: LEMONS_HAS_BINARY_DATA
 
    Set this to 1 if your project has a juce binary data target.
    The Lemons repo's cmake scripts set this automatically for you; if that's how you added the Lemons package, then you don't need to worry about setting this manually.
 */
#ifndef LEMONS_HAS_BINARY_DATA
#    define LEMONS_HAS_BINARY_DATA 0
#endif


//==============================================================================
/** Config: LEMONS_USE_VDSP
 
    Set this to 1 to use Apple's vDSP library for vecops SIMD.
    vDSP ships with the OS on Mac and iOS, which is why it's the default on Apple platforms.
    The Lemons repo's cmake scripts set this automatically for you; if that's how you added the Lemons package, then you don't need to worry about setting this manually.
 */
#ifndef LEMONS_USE_VDSP
#    if JUCE_IOS || JUCE_MAC
#        define LEMONS_USE_VDSP 1
#    else
#        define LEMONS_USE_VDSP 0
#    endif
#endif


/*=======================================================================*/

#include <juce_audio_utils/juce_audio_utils.h>

#include "misc/Preprocessor.h"


namespace bav
{
using juce::File;
using juce::MidiBuffer;
using juce::MidiMessage;
using juce::String;
using juce::ValueTree;

template < typename T >
using AudioBuffer = juce::AudioBuffer< T >;

template < typename T >
using Array = juce::Array< T >;

template < typename T >
using OwnedArray = juce::OwnedArray< T >;

}  // namespace bav


#include "misc/misc.h"
#include "misc/ValueSmoother.h"
#include "misc/TypeTraits.h"
#include "misc/WrapperType.h"

#include "events/Broadcaster.h"
#include "events/Listener.h"
#include "events/Timers.h"

#include "files/FileUtilities.h"

#include "binary_data/BinaryDataHelpers.h"

#include "localization/localization.h"

#include "math/mathHelpers.h"
#include "math/vecops/vecops.h"
