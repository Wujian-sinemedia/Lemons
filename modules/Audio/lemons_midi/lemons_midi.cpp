#include "lemons_midi.h"


#include "utilities/MidiFIFO.cpp"
#include "utilities/VelocityHelper.cpp"
#include "utilities/MidiUtilities.cpp"

#include "processors/MidiProcessor.cpp"
#include "processors/MidiChoppingProcessor.cpp"

#include <libMTSClient.cpp>
#include "tuning/mts_client.cpp"
#include "tuning/PitchbendTracker.cpp"
#include "tuning/PitchPipeline.cpp"
