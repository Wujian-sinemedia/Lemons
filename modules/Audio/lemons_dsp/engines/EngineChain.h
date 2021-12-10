#pragma once

namespace lemons::dsp
{

template <typename SampleType>
class EngineChain final : public LatencyEngine<SampleType>
{
public:

	struct Node
	{
		explicit Node (std::unique_ptr<Engine<SampleType>> engineToUse);

		~Node() = default;

		std::unique_ptr<Engine<SampleType>> engine;
		int                                 index { 0 };
		bool                                bypassed { false };
	};

	Node*       getNode (int index);
	const Node* getNode (int index) const;

	template <typename EngineType, LEMONS_MUST_INHERIT_FROM (EngineType, Engine<SampleType>)>
	Node* getNodeOfType()
	{
		for (auto* node : nodes)
			if (const auto* typed = dynamic_cast<EngineType*> (node->engine.get()))
				return node;

		return nullptr;
	}

	template <typename EngineType, LEMONS_MUST_INHERIT_FROM (EngineType, Engine<SampleType>)>
	EngineType* getEngineOfType()
	{
		for (auto* node : nodes)
			if (auto* engine = dynamic_cast<EngineType*> (node->engine.get()))
				return engine;

		return nullptr;
	}

	void addNode (Node* newNode);

	template <typename EngineType, typename... Args, LEMONS_MUST_INHERIT_FROM (EngineType, Engine<SampleType>)>
	Node* createAndAddNode (Args&&... args)
	{
		auto* node = new Node (std::make_unique<EngineType> (std::forward<Args> (args)...));

		node->index = getNextNodeIndex();

		return nodes.add (node);
	}

private:
	void renderChunk (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midiMessages, bool isBypassed) final;

	void onPrepare (int blocksize, double samplerate, int numChannels) final;

	void onRelease() final;

	int getNextNodeIndex() const;

	juce::OwnedArray<Node> nodes;
};

}  // namespace lemons::dsp
