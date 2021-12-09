#pragma once

namespace lemons::dsp
{

template <typename SampleType>
class EngineChain final : public LatencyEngine<SampleType>
{
public:

	struct Node
	{
		std::unique_ptr<Engine<SampleType>> engine;
		int                                 index { 0 };
		bool                                bypassed { false };
	};


	Node*       getNode (int index);
	const Node* getNode (int index) const;

	int getNextNodeIndex() const;

private:
	void renderChunk (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midiMessages, bool isBypassed) final;

	void onPrepare (int blocksize, double samplerate, int numChannels) final;

	void onRelease() final;

	juce::OwnedArray<Node> nodes;
};

}  // namespace lemons::dsp
