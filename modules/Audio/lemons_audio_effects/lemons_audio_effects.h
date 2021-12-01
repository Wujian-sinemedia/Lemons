#pragma once

/*-------------------------------------------------------------------------------------

BEGIN_JUCE_MODULE_DECLARATION

ID:                 lemons_audio_effects
vendor:             Lemons
version:            0.0.1
name:               lemons_audio_effects
description:        Audio effects
website:            http://benthevining.github.io/Lemons/
license:            GPL-3.0
minimumCppStandard: 17
dependencies:       lemons_dsp juce_dsp

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

#include "panning/MonoToStereoPanner.h"
#include "panning/StereoPanner.h"

#include "time/Delay.h"

#include "EQ/Filter.h"
#include "EQ/EQ.h"
