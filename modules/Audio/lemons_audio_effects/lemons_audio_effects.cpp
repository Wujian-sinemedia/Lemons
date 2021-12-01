#include "lemons_audio_effects.h"


#include "AudioEffects/AudioEffect.cpp"

#include "dynamics/Compressor.cpp"
#include "dynamics/Limiter.cpp"
#include "dynamics/NoiseGate.cpp"
#include "dynamics/SmoothedGain.cpp"
#include "misc/DeEsser.cpp"
#include "misc/DryWet.cpp"
#include "misc/Reverb.cpp"
#include "panning/MonoToStereoPanner.cpp"
#include "panning/StereoPanner.cpp"
#include "panning/PannerBase.cpp"
#include "time/Delay.cpp"

#include "EQ/Filter.cpp"
#include "EQ/EQ.cpp"
