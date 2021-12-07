#pragma once

namespace lemons::midi
{
template <typename SampleType>
class MidiChoppingProcessor
{
public:
	using AudioBuffer = juce::AudioBuffer<SampleType>;

	virtual ~MidiChoppingProcessor() = default;

	void prepare (int maxBlocksize);

	void process (AudioBuffer& audio, MidiBuffer& midi);

	void processBypassed (const MidiBuffer& midi);

private:
	void processInternal (AudioBuffer& audio, MidiBuffer& midi,
	                      int startSample, int numSamples);

	virtual void handleMidiMessage (const MidiMessage& m)           = 0;
	virtual void renderChunk (AudioBuffer& audio, MidiBuffer& midi) = 0;

	MidiBuffer midiStorage;
};

}  // namespace lemons::midi
