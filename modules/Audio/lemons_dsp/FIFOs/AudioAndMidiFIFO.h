#pragma once

#include <lemons_midi/lemons_midi.h>

namespace lemons::dsp
{
template <typename SampleType>
class AudioAndMidiFIFO
{
public:
	AudioAndMidiFIFO (int samples = 1024);

	void setSize (int numSamples);

	void push (const AudioBuffer<SampleType>& audioIn, const MidiBuffer& midiIn);

	void pop (AudioBuffer<SampleType>& audioOut, MidiBuffer& midiOut);

	int numStoredSamples() const;

private:
	midi::MidiFIFO             midi;
	MultiAudioFIFO<SampleType, 2> audio;
};

}  // namespace lemons::dsp
