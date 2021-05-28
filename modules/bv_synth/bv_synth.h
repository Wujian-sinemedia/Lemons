
/*******************************************************************************
 BEGIN_JUCE_MODULE_DECLARATION
 ID:                 bv_SynthBase
 vendor:             Ben Vining
 version:            0.0.1
 name:               SynthBase
 description:        Generalized infrastructure for a polyphonic synthesiser instrument
 dependencies:       bv_dsp
 END_JUCE_MODULE_DECLARATION
 *******************************************************************************/


#pragma once

#include "bv_dsp/bv_dsp.h"


/*
    MTS-ESP is an open source library that allows one master plugin in a session to communicate microtuning to all the clients in the session.
    To use MTS-ESP with this module, link to the library and define this macro to 1. Your synth will act as an MTS-ESP client, querying the master plugin for note filtering and retuning information.
    If you don't use MTS-ESP, you can still retune your synthesizer using the setConcertPitchHz() method.
    MTS-ESP can be found at http://github.com/ODDSound/MTS-ESP.     
*/
#ifndef bvsb_USE_MTS_ESP
#    define bvsb_USE_MTS_ESP 0
#endif

#if bvsb_USE_MTS_ESP
#    include <MTS-ESP/Client/libMTSClient.h>
#endif

#include "SynthVoice/SynthVoice.h"

#include "Synth/Synth.h"

#include "sine_synth.h"
