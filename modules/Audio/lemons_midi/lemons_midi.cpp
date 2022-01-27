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


#include "lemons_midi.h"


#include "unit_tests/midi_tests.cpp"

#include "utilities/MidiFIFO.cpp"
#include "utilities/MidiUtilities.cpp"
#include "utilities/VelocityHelper.cpp"
#include "utilities/serializing.cpp"

#include "processors/MidiChoppingProcessor.cpp"
#include "processors/processors.cpp"

#include "tuning/PitchPipeline.cpp"
#include "tuning/PitchbendTracker.cpp"
#include "tuning/mts_client.cpp"
#include <libMTSClient.cpp>

#if LEMONS_UNIT_TESTING
#	include "utilities/MidiFIFO_tests.cpp"
#	include "utilities/serializing_tests.cpp"
#endif
