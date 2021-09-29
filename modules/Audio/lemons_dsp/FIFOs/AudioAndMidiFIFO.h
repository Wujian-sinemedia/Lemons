#pragma once

#include <lemons_midi/lemons_midi.h>

namespace lemons::dsp
{
template < typename SampleType >
class AudioAndMidiFIFO
{
public:
    AudioAndMidiFIFO (int channels = 2, int samples = 1024);

    void setSize (int numChannels, int numSamples);

    void push (const AudioBuffer< SampleType >& audioIn, const MidiBuffer& midiIn);

    void pop (AudioBuffer< SampleType >& audioOut, MidiBuffer& midiOut);

    int numStoredSamples() const;

private:
    midi::MidiFIFO               midi;
    MultiAudioFIFO< SampleType > audio;
};

}  // namespace lemons::dsp
