
#include "bv_mts_esp_client.h"

#if BV_USE_MTS_ESP
#    include "mts_esp_client/mts_esp_client.cpp"
#else
#    include "fallback/fallback_pitch_converter.cpp"
#endif


namespace bav
{
float PitchConverter::midiToFrequency (int midiNote, int midiChannel) const
{
#if BV_USE_MTS_ESP
    return mtsClient.getFrequencyForNote (midiNote, midiChannel);
#else
    juce::ignoreUnused (midiChannel);
    return converter.midiToFrequency (midiNote);
#endif
}

float PitchConverter::midiToFrequency (float midiNote, int midiChannel) const
{
#if BV_USE_MTS_ESP
    return mtsClient.getFrequencyForNote (midiNote, midiChannel);
#else
    juce::ignoreUnused (midiChannel);
    return converter.midiToFrequency (midiNote);
#endif
}

int PitchConverter::frequencyToMidi (float frequency, int midiChannel) const
{
#if BV_USE_MTS_ESP
    return mtsClient.getNoteForFrequency (frequency, midiChannel);
#else
    juce::ignoreUnused (midiChannel);
    return converter.frequencyToMidi (frequency);
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

bool PitchConverter::setConcertPitchHz (float newConcertPitchhz)
{
#if BV_USE_MTS_ESP
    juce::ignoreUnused (newConcertPitchhz);
    return false;
#else
    return converter.setConcertPitchHz (newConcertPitchhz);
#endif
}


}  // namespace bav
