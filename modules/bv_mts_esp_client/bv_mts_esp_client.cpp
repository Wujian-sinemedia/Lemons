
#include "bv_mts_esp_client.h"

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

#    include <libMTSClient.cpp>

#    ifdef __clang__
#        pragma clang diagnostic pop
#    elif defined __GNUC__
#        pragma GCC diagnostic pop
#    elif defined _MSC_VER
#        pragma warning(pop)
#    endif


namespace bav
{

MTSESPClient::MTSESPClient()
{
    client = MTS_RegisterClient();
    jassert (client != nullptr);
}

MTSESPClient::~MTSESPClient()
{
    MTS_DeregisterClient (client);
}

float MTSESPClient::getFrequencyForNote (int midiPitch, int midiChannel) const
{
    return static_cast<float> (MTS_NoteToFrequency (client, char (midiPitch), char (midiChannel)));
}

float MTSESPClient::getFrequencyForNote (float midiPitch, int midiChannel) const
{
    return static_cast<float> (MTS_NoteToFrequency (client, char (midiPitch), char (midiChannel)));
}

int MTSESPClient::getNoteForFrequency (float frequency, int midiChannel) const
{
    return int (MTS_FrequencyToNote (client, double(frequency), char (midiChannel)));
}

bool MTSESPClient::shouldFilterNote (int midiPitch, int midiChannel) const
{
    return MTS_ShouldFilterNote (client, char (midiPitch), char (midiChannel));
}

bool MTSESPClient::isConnected() const
{
    return MTS_HasMaster (client);
}

juce::String MTSESPClient::getScaleName() const
{
    return {MTS_GetScaleName (client)};
}

}  // namespace

#endif /* BV_USE_MTS_ESP */


namespace bav
{

float PitchConverter::midiToFrequency (int midiNote, int midiChannel) const
{
#if BV_USE_MTS_ESP
    return mtsClient.getFrequencyForNote (midiNote, midiChannel);
#else
    juce::ignoreUnused (midiChannel);
    return converter.mtof (midiNote);
#endif
}

float PitchConverter::midiToFrequency (float midiNote, int midiChannel) const
{
#if BV_USE_MTS_ESP
    return mtsClient.getFrequencyForNote (midiNote, midiChannel);
#else
    juce::ignoreUnused (midiChannel);
    return converter.mtof (midiNote);
#endif
}

int PitchConverter::frequencyToMidi (float frequency, int midiChannel) const
{
#if BV_USE_MTS_ESP
    return mtsClient.getNoteForFrequency (frequency, midiChannel);
#else
    juce::ignoreUnused (midiChannel);
    return juce::roundToInt (converter.ftom (frequency));
#endif
}

bool PitchConverter::shouldFilterNote (int midiPitch, int midiChannel) const
{
#if BV_USE_MTS_ESP
    return mtsClient.shouldFilterNote (midiPitch, midiChannel);
#else
    juce::ignoreUnused (midiPitch, midiChannel);
    return false;
#endif
}

bool PitchConverter::isConnectedToMtsEsp() const
{
#if BV_USE_MTS_ESP
    return mtsClient.isConnected();
#else
    return false;
#endif
}

juce::String PitchConverter::getScaleName() const
{
#if BV_USE_MTS_ESP
    return mtsClient.getScaleName();
#else
    return {};
#endif
}

bool PitchConverter::setConcertPitchHz (const int newConcertPitchhz)
{
#if BV_USE_MTS_ESP
    juce::ignoreUnused (newConcertPitchhz);
    return false;
#else
    if (converter.getCurrentConcertPitchHz() == newConcertPitchhz) return false;
    converter.setConcertPitchHz (newConcertPitchhz);
    return true;
#endif
}


}  // namespace
