
#include "bv_dsp.h"

#include "FIFOs/AudioFIFO.cpp"
#include "Oscillators/oscillators.cpp"
#include "Oscillators/LFO/LFO.cpp"
#include "FIFOWrappedEngine/FIFOWrappedEngine.cpp"
#include "PitchDetector/pitch-detector.cpp"
#include "PSOLA/analysis/psola_analyzer.cpp"
#include "PSOLA/resynthesis/psola_shifter.cpp"

#include "synth/SynthVoice/SynthVoice.cpp"
#include "synth/internals/SynthMidi.cpp"
#include "synth/internals/SynthParameters.cpp"
#include "synth/internals/SynthVoiceAllocation.cpp"
#include "synth/Synth.cpp"
