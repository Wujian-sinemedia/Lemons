#pragma once

#include <libMTSClient.h>


namespace lemons::midi
{
class PitchConverter
{
public:
    /*
     MTS-ESP supports specific mappings for each midi channel; pass -1 for "unspecified" or "all channels".
     */
    float midiToFrequency (int midiNote, int midiChannel = -1) const;
    float midiToFrequency (float midiNote, int midiChannel = -1) const;

    int frequencyToMidi (float frequency, int midiChannel = -1) const;

    bool shouldFilterNote (int midiPitch, int midiChannel = -1) const;

    bool isConnectedToMtsEsp() const;

    String getScaleName() const;

private:
    struct Deleter
    {
        void operator() (MTSClient* c);
    };

    std::unique_ptr< MTSClient, Deleter > client {MTS_RegisterClient()};
};


}  // namespace lemons::midi
