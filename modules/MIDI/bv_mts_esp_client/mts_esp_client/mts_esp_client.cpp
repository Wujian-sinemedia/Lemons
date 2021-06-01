
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


namespace bav::midi
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
    return static_cast< float > (MTS_NoteToFrequency (client, char (midiPitch), char (midiChannel)));
}

float MTSESPClient::getFrequencyForNote (float midiPitch, int midiChannel) const
{
    return static_cast< float > (MTS_NoteToFrequency (client, char (midiPitch), char (midiChannel)));
}

int MTSESPClient::getNoteForFrequency (float frequency, int midiChannel) const
{
    return int (MTS_FrequencyToNote (client, double (frequency), char (midiChannel)));
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

}  // namespace bav
