/*******************************************************************************
 BEGIN_JUCE_MODULE_DECLARATION
 ID:                 bv_mts_esp_client
 vendor:             Ben Vining
 version:            0.0.1
 name:               bv_mts_esp_client
 description:        Interface that wraps the MTS-ESP library
 dependencies:       bv_midi
 END_JUCE_MODULE_DECLARATION
 *******************************************************************************/

#pragma once

#include <bv_midi/bv_midi.h>


#ifndef BV_USE_MTS_ESP
#    define BV_USE_MTS_ESP 0
#endif


#if BV_USE_MTS_ESP

#    ifdef __clang__
#        pragma clang diagnostic push
#        pragma clang diagnostic ignored "-Weverything"
#    elif defined __GNUC__
#        pragma GCC diagnostic push
#        pragma GCC diagnostic ignored "-Weverything"
#    elif defined _MSC_VER
#        pragma warning(push, 0)
#    endif

#    include <libMTSClient.h>

#    ifdef __clang__
#        pragma clang diagnostic pop
#    elif defined __GNUC__
#        pragma GCC diagnostic pop
#    elif defined _MSC_VER
#        pragma warning(pop)
#    endif


namespace bav
{

struct MTSESPClient
{
    MTSESPClient();
    
    virtual ~MTSESPClient();
    
    float getFrequencyForNote (int   midiPitch, int midiChannel = -1) const;
    float getFrequencyForNote (float midiPitch, int midiChannel = -1) const;
    
    int getNoteForFrequency (float frequency, int midiChannel = -1) const;
    
    bool shouldFilterNote (int midiPitch, int midiChannel = -1) const;
    
    bool isConnected() const;
    
    juce::String getScaleName() const;
    
private:
    MTSClient* client;
};

}  // namespace

#endif /* BV_USE_MTS_ESP */


namespace bav
{


/*
 This class provides a consistent interface for building with or without MTS-ESP, and all of its methods are safe to call whether or not you're using MTS-ESP.
 */
class PitchConverter
{
public:
    float midiToFrequency (int   midiNote, int midiChannel = -1) const;
    float midiToFrequency (float midiNote, int midiChannel = -1) const;
    
    int frequencyToMidi (float frequency, int midiChannel = -1) const;
    
    bool shouldFilterNote (int midiPitch, int midiChannel = -1) const;
    
    bool isConnectedToMtsEsp() const;
    
    juce::String getScaleName() const;
    
    /* If you are not using MTS-ESP, you can call this method to alter the master tuning (the frequency of A4). If you are using MTS-ESP, calling this method does nothing.
       Returns true if this operation had any effect. */
    bool setConcertPitchHz (const int newConcertPitchhz);
    
private:
#if BV_USE_MTS_ESP
    MTSESPClient mtsClient;
#else
    midi::PitchConverter converter {440, 69, 12};
#endif
};


}  // namespace
