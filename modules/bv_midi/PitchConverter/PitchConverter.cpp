
#if BV_USE_MTS_ESP

#ifdef __clang__
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Weverything"
#elif defined __GNUC__
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Weverything"
#elif defined _MSC_VER
#    pragma warning(push, 0)
#endif

#include <libMTSClient.cpp>

#ifdef __clang__
#    pragma clang diagnostic pop
#elif defined __GNUC__
#    pragma GCC diagnostic pop
#elif defined _MSC_VER
#    pragma warning(pop)
#endif

#endif /* if BV_USE_MTS_ESP */


namespace bav::midi
{

PitchConverter::PitchConverter()
{
#if BV_USE_MTS_ESP
    client = MTS_RegisterClient();
    jassert (client != nullptr);
#endif
}

PitchConverter::~PitchConverter()
{
#if BV_USE_MTS_ESP
    MTS_DeregisterClient (client);
#endif
}

float PitchConverter::midiToFrequency (int midiNote, int midiChannel) const
{
#if BV_USE_MTS_ESP
    return static_cast< float > (MTS_NoteToFrequency (client, char (midiNote), char (midiChannel)));
#else
    juce::ignoreUnused (midiChannel);
    return static_cast< float > (concertPitchHz
                                 * std::pow (2.0f,
                                             ((static_cast<float>(midiNote) - 69.0f) / 12.0f)));
#endif
}

float PitchConverter::midiToFrequency (float midiNote, int midiChannel) const
{
#if BV_USE_MTS_ESP
    return static_cast< float > (MTS_NoteToFrequency (client, char (midiNote), char (midiChannel)));
#else
    juce::ignoreUnused (midiChannel);
    return static_cast< float > (concertPitchHz
                                 * std::pow (2.0f,
                                             ((midiNote - 69.0f) / 12.0f)));
#endif
}

int PitchConverter::frequencyToMidi (float frequency, int midiChannel) const
{
#if BV_USE_MTS_ESP
    return juce::roundToInt (MTS_FrequencyToNote (client, static_cast<double>(frequency), char (midiChannel)));
#else
    juce::ignoreUnused (midiChannel);
    return juce::roundToInt (12.0f
                             * log2 (frequency / concertPitchHz)
                             + 69.0f);
#endif
}

bool PitchConverter::shouldFilterNote (int midiPitch, int midiChannel) const
{
#if BV_USE_MTS_ESP
    return MTS_ShouldFilterNote (client, char (midiPitch), char (midiChannel));
#else
    juce::ignoreUnused (midiPitch, midiChannel);
    return false;
#endif
}

bool PitchConverter::isConnectedToMtsEsp() const
{
#if BV_USE_MTS_ESP
    return MTS_HasMaster (client);
#else
    return false;
#endif
}

juce::String PitchConverter::getScaleName() const
{
#if BV_USE_MTS_ESP
    return {MTS_GetScaleName (client)};
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
    if (concertPitchHz == newConcertPitchHz)
        return false;
    
    concertPitchHz = newConcertPitchHz;
    return true;
#endif
}


}  // namespace bav
