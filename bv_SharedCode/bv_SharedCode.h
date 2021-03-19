/*******************************************************************************
 BEGIN_JUCE_MODULE_DECLARATION
 ID:                 bv_SharedCode
 vendor:             Ben Vining
 version:            0.0.1
 name:               Ben Vining's codebase
 description:        General utilities useful for developing plugins.
 dependencies:       juce_audio_utils, juce_dsp
 END_JUCE_MODULE_DECLARATION
 *******************************************************************************/


#pragma once

// dependencies
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_dsp/juce_dsp.h>

// the rest of this module
#include "misc/System.h"
#include "misc/Parameters.h"
#include "misc/MessageQueue.h"
#include "math/mathHelpers.h"
#include "math/intOps.h"
#include "math/VectorOps.h"
#include "midi/MidiFIFO.h"
#include "midi/MidiUtilities.h"
#include "dsp/AudioFIFO.h"
#include "dsp/Panner.h"
#include "dsp/FX/NoiseGate.h"
#include "dsp/FX/Compressor.h"
#include "dsp/FX/Limiter.h"
#include "dsp/FX/DeEsser.h"
#include "dsp/FX/Reverb.h"
#include "dsp/FIFOWrappedEngine.h"


namespace bav::gui
{
    
    
    inline juce::Button::ButtonState buttonStateFromBool (const bool isOn)
    {
        if (isOn)
            return juce::Button::ButtonState::buttonDown;

        return juce::Button::ButtonState::buttonNormal;
    }
    

}  // namespace
