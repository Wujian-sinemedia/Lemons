
#pragma once

#ifdef __clang__
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Weverything"
#elif defined __GNUC__
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Weverything"
#elif defined _MSC_VER
#    pragma warning(push, 0)
#endif

#include <libMTSClient.h>

#ifdef __clang__
#    pragma clang diagnostic pop
#elif defined __GNUC__
#    pragma GCC diagnostic pop
#elif defined _MSC_VER
#    pragma warning(pop)
#endif


namespace bav
{
class MTSESPClient
{
public:
    MTSESPClient();

    virtual ~MTSESPClient();

    float getFrequencyForNote (int midiPitch, int midiChannel = -1) const;
    float getFrequencyForNote (float midiPitch, int midiChannel = -1) const;

    int getNoteForFrequency (float frequency, int midiChannel = -1) const;

    bool shouldFilterNote (int midiPitch, int midiChannel = -1) const;

    bool isConnected() const;

    juce::String getScaleName() const;

private:
    MTSClient* client;
};

}  // namespace bav
