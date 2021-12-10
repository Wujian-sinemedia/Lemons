namespace lemons::dsp
{

template <typename SampleType>
EngineChain<SampleType>::Node::Node (std::unique_ptr<Engine<SampleType>> engineToUse)
    : engine (std::move (engineToUse))
{
	jassert (engine.get() != nullptr);
}

template <typename SampleType>
Engine<SampleType>& EngineChain<SampleType>::Node::getEngine()
{
	return *engine.get();
}

template <typename SampleType>
bool EngineChain<SampleType>::Node::isBypassed() const noexcept
{
	return bypassed;
}

template <typename SampleType>
void EngineChain<SampleType>::Node::setBypassed (bool shouldBeBypassed) noexcept
{
	bypassed = shouldBeBypassed;
}

template <typename SampleType>
int EngineChain<SampleType>::Node::getIndex() const noexcept
{
	return index;
}

/*-----------------------------------------------------------------------------------------------------------------*/

template <typename SampleType>
typename EngineChain<SampleType>::Node* EngineChain<SampleType>::getNode (int index)
{
	for (auto* node : nodes)
		if (node->index == index)
			return node;

	return nullptr;
}

template <typename SampleType>
const typename EngineChain<SampleType>::Node* EngineChain<SampleType>::getNode (int index) const
{
	for (const auto* node : nodes)
		if (node->index == index)
			return node;

	return nullptr;
}

template <typename SampleType>
int EngineChain<SampleType>::getNextNodeIndex() const
{
	int idx = -1;

	for (const auto* node : nodes)
		idx = std::max (idx, node->index);

	return idx + 1;
}

template <typename SampleType>
void EngineChain<SampleType>::renderChunk (const AudioBuffer<SampleType>& input,
                                           AudioBuffer<SampleType>& output, MidiBuffer& midiMessages, bool isBypassed)
{
	for (int i = 0; i < getNextNodeIndex(); ++i)
	{
		if (auto* node = getNode (i))
		{
			const auto nodeBypass = node->bypassed ? true : isBypassed;

			node->engine->process (input, output, midiMessages, nodeBypass);
		}
	}
}

template <typename SampleType>
void EngineChain<SampleType>::onPrepare (int blocksize, double samplerate, int numChannels)
{
	int latency = 0;

	for (const auto* node : nodes)
		latency = std::max (latency, node->engine->reportLatency());

	if (latency != this->reportLatency())
	{
		this->changeLatency (latency);
		return;
	}

	for (auto* node : nodes)
		node->engine->prepare (samplerate, blocksize, numChannels);
}

template <typename SampleType>
void EngineChain<SampleType>::onRelease()
{
	for (auto* node : nodes)
		node->engine->releaseResources();
}

template <typename SampleType>
int EngineChain<SampleType>::addNode (Node* newNode)
{
	if (nodes.contains (newNode))
	{
		jassertfalse;
		return 0;
	}

	newNode->index = getNextNodeIndex();
	return nodes.add (newNode)->index;
}

template <typename SampleType>
void EngineChain<SampleType>::addNodeToFront (Node* newNode)
{
	if (nodes.contains (newNode))
		jassertfalse;

	newNode->index = 0;

	for (auto* node : nodes)
		node->index += 1;

	nodes.add (newNode);
}

template <typename SampleType>
int EngineChain<SampleType>::addNodeAfter (Node* newNode, const Node* nodeToInsertAfter)
{
	if (! nodes.contains (nodeToInsertAfter))
	{
		jassertfalse;
		return 0;
	}

	newNode->index = nodeToInsertAfter->index + 1;

	for (int i = newNode->index; i < getNextNodeIndex(); ++i)
		if (auto* node = getNode (i))
			node->index += 1;

	return nodes.add (newNode)->index;
}

template <typename SampleType>
int EngineChain<SampleType>::addNodeBefore (Node* newNode, const Node* nodeToInsertBefore)
{
	if (! nodes.contains (nodeToInsertBefore))
	{
		jassertfalse;
		return 0;
	}

	newNode->index = std::max (0, nodeToInsertBefore->index - 1);

	for (int i = newNode->index; i < getNextNodeIndex(); ++i)
		if (auto* node = getNode (i))
			node->index += 1;

	return nodes.add (newNode)->index;
}

template <typename SampleType>
bool EngineChain<SampleType>::swapNodes (Node* node1, Node* node2)
{
	if (! nodes.contains (node1) || ! nodes.contains (node2))
		return false;

	const auto prevIdx1 = node1->index;
	const auto prevIdx2 = node2->index;

	jassert (prevIdx1 != prevIdx2);

	node2->index = prevIdx1;
	node1->index = prevIdx2;

	return true;
}

template <typename SampleType>
bool EngineChain<SampleType>::swapNodesAtIndices (int index1, int index2)
{
	if (index1 == index2)
		return false;

	if (auto* node1 = getNode (index1))
	{
		if (auto* node2 = getNode (index2))
		{
			swapNodes (node1, node2);
			return true;
		}
	}

	return false;
}

template <typename SampleType>
bool EngineChain<SampleType>::removeNode (Node* node)
{
	if (! nodes.contains (node))
		return false;

	nodes.removeObject (node);
	return true;
}

template <typename SampleType>
bool EngineChain<SampleType>::removeNode (int index)
{
	if (auto* node = getNode (index))
	{
		nodes.removeObject (node);
		return true;
	}

	return false;
}

template class EngineChain<float>;
template class EngineChain<double>;

}  // namespace lemons::dsp
