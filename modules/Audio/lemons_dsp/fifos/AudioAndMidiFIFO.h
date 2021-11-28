#pragma once

#include <lemons_midi/lemons_midi.h>

namespace lemons::dsp
{
template <typename SampleType>
class AudioAndMidiFIFO
{
public:
	AudioAndMidiFIFO (int samples = 1024);

	void setSize (int numSamples, int numChannels = 2);

	void push (const AudioBuffer<SampleType>& audioIn, const MidiBuffer& midiIn);

	void pop (AudioBuffer<SampleType>& audioOut, MidiBuffer& midiOut);

	int numStoredSamples() const noexcept;

	void clear();

private:
	midi::MidiFIFO        midi;
	AudioFifo<SampleType> audio;
};

}  // namespace lemons::dsp
