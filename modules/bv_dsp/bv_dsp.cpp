
#include "bv_dsp.h"

#include "FIFOs/AudioFIFO.cpp"

#include "Oscillators/oscillators.cpp"
#include "Oscillators/choosable/ChoosableOscillator.cpp"
#include "Oscillators/choosable/OscEngine.cpp"

#include "engines/AudioEngine.cpp"
#include "engines/LatencyEngine.cpp"
#include "MidiChoppingProcessor/MidiChoppingProcessor.cpp"

#include "PitchDetector/pitch-detector.cpp"
#include "PSOLA/analysis/psola_analyzer.cpp"
#include "PSOLA/resynthesis/psola_shifter.cpp"

#include "synth/SynthVoice/SynthVoice.cpp"
#include "synth/internals/SynthMidi.cpp"
#include "synth/internals/SynthParameters.cpp"
#include "synth/internals/SynthVoiceAllocation.cpp"
#include "synth/Synth.cpp"

#include "FX/ReorderableFxChain.cpp"
#include "FX/dynamics/Compressor.cpp"
#include "FX/dynamics/Limiter.cpp"
#include "FX/dynamics/NoiseGate.cpp"
#include "FX/dynamics/SmoothedGain.cpp"
#include "FX/misc/DeEsser.cpp"
#include "FX/misc/DryWet.cpp"
#include "FX/misc/Reverb.cpp"
#include "FX/stereo_image/MonoStereoConverter.cpp"
#include "FX/stereo_image/panning/PannerBase.cpp"
#include "FX/stereo_image/panning/MonoToStereoPanner.cpp"
#include "FX/stereo_image/panning/StereoPanner.cpp"
#include "FX/time/Delay.cpp"

#include "filters/Coeffecients.cpp"
#include "filters/Filter.cpp"
