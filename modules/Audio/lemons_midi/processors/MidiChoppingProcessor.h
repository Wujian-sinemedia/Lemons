#pragma once

namespace lemons::midi
{

template <typename T>
using AudioBuffer = juce::AudioBuffer<T>;

template <typename SampleType>
class ChoppingProcessor
{
public:
	virtual ~ChoppingProcessor() = default;

	void prepare (int maxBlocksize);

	void process (AudioBuffer<SampleType>& audio, MidiBuffer& midi);

	void releaseResources();

private:
	void processInternal (AudioBuffer<SampleType>& audio, MidiBuffer& midi,
	                      int startSample, int endSample);

	[[nodiscard]] virtual bool shouldChopAroundMidiMessage (const MidiMessage& m) const;

	virtual void handleMidiMessage (const MidiMessage& m)                       = 0;
	virtual void renderChunk (AudioBuffer<SampleType>& audio, MidiBuffer& midi) = 0;

	MidiBuffer midiStorage;
};

}  // namespace lemons::midi
