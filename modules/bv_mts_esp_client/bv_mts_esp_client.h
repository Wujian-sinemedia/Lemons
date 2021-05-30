/*******************************************************************************
 BEGIN_JUCE_MODULE_DECLARATION
 ID:                 bv_mts_esp_client
 vendor:             Ben Vining
 version:            0.0.1
 name:               bv_mts_esp_client
 description:        Interface that wraps the MTS-ESP library
 dependencies:   juce_core
 END_JUCE_MODULE_DECLARATION
 *******************************************************************************/

#pragma once

#include <juce_core/juce_core.h>


#ifndef BV_USE_MTS_ESP
#    define BV_USE_MTS_ESP 0
#endif


#if BV_USE_MTS_ESP
#    include "mts_esp_client/mts_esp_client.h"
#else
#    include "fallback/fallback_pitch_converter.h"
#endif


namespace bav
{
/*
 This class provides a consistent interface for building with or without MTS-ESP, and all of its methods are safe to call whether or not you're using MTS-ESP.
 */
class PitchConverter
{
public:
    /*
     MTS-ESP supports specific mappings for each midi channel; pass -1 for "unspecified" or "all channels". In the fallback version, the midi channel is ignored.
     */
    float midiToFrequency (int midiNote, int midiChannel = -1) const;
    float midiToFrequency (float midiNote, int midiChannel = -1) const;

    int frequencyToMidi (float frequency, int midiChannel = -1) const;

    /* This always returns false if you are not using MTS-ESP. */
    bool shouldFilterNote (int midiPitch, int midiChannel = -1) const;

    bool isConnectedToMtsEsp() const;

    /* This returns an empty string if you are not using MTS-ESP, or if you are and there is no scale name specified. */
    juce::String getScaleName() const;

    /* If you are not using MTS-ESP, you can call this method to alter the master tuning (the frequency of A4). If you are using MTS-ESP, calling this method does nothing.
       Returns true if this operation had any effect. */
    bool setConcertPitchHz (float newConcertPitchhz);

private:
#if BV_USE_MTS_ESP
    MTSESPClient mtsClient;
#else
    FallbackPitchConverter converter;
#endif
};


}  // namespace bav
