
/** @ingroup lemons_audio_effects
 *  @{
 */
#include "lemons_audio_effects.h"


#include "AudioEffects/AudioEffect.cpp"

#include "dynamics/Compressor.cpp"
#include "dynamics/Limiter.cpp"
#include "dynamics/NoiseGate.cpp"
#include "dynamics/SmoothedGain.cpp"
#include "misc/DeEsser.cpp"
#include "misc/DryWet.cpp"
#include "misc/Reverb.cpp"
#include "stereo_image/MonoStereoConverter.cpp"
#include "stereo_image/panning/MonoToStereoPanner.cpp"
#include "stereo_image/panning/StereoPanner.cpp"
#include "stereo_image/panning/PannerBase.cpp"
#include "time/Delay.cpp"

#include "EQ/Filter.cpp"
#include "EQ/EQ.cpp"

/** @}*/
