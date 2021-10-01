#pragma once

/*-------------------------------------------------------------------------------------

BEGIN_JUCE_MODULE_DECLARATION

ID:                 lemons_audio_effects
vendor:             Ben Vining
version:            0.0.1
name:               lemons_audio_effects
description:        Audio effects
dependencies:       lemons_dsp lemons_serializing

END_JUCE_MODULE_DECLARATION
 
-------------------------------------------------------------------------------------*/


#include "AudioEffects/AudioEffect.h"
#include "AudioEffects/AudioEffectEngine.h"

#include "dynamics/SmoothedGain.h"
#include "dynamics/NoiseGate.h"
#include "dynamics/Compressor.h"
#include "dynamics/Limiter.h"

#include "misc/DeEsser.h"
#include "misc/Reverb.h"
#include "misc/DryWet.h"

#include "stereo_image/MonoStereoConverter.h"
#include "stereo_image/panning/MonoToStereoPanner.h"
#include "stereo_image/panning/StereoPanner.h"

#include "time/Delay.h"

#include "EQ/Filter.h"
#include "EQ/EQ.h"
