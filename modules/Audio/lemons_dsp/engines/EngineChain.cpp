
/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

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
Engine<SampleType>* EngineChain<SampleType>::Node::operator->()
{
	return engine.get();
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
	return alg::contains_or_null<Node> (nodes,
										[index] (Node* n)
										{ return n->index == index; });
}

template <typename SampleType>
const typename EngineChain<SampleType>::Node* EngineChain<SampleType>::getNode (int index) const
{
	return alg::contains_or_null<Node> (nodes,
										[index] (Node* n)
										{ return n->index == index; });
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
	for (auto i = 0; i < getNextNodeIndex(); ++i)
	{
		if (auto* node = getNode (i))
		{
			const auto nodeBypass = node->bypassed ? true : isBypassed;

			node->engine->process (input, output, midiMessages, nodeBypass);
		}
	}
}

template <typename SampleType>
void EngineChain<SampleType>::prepared (int blocksize, double samplerate, int numChannels)
{
	int latency = 0;

	for (const auto* node : nodes)
		latency = std::max (latency, node->engine->reportLatency());

	if (latency != this->reportLatency())
		this->changeLatency (latency);

	for (auto* node : nodes)
		node->engine->prepare (samplerate, blocksize, numChannels);
}

template <typename SampleType>
void EngineChain<SampleType>::latencyChanged (int newLatency)
{
	for (auto* node : nodes)
		if (auto* latencyEngine = dynamic_cast<LatencyEngine<SampleType>*> (node->engine.get()))
			latencyEngine->changeLatency (newLatency);
}

template <typename SampleType>
void EngineChain<SampleType>::onReset()
{
	for (auto* node : nodes)
		node->engine->reset();
}

template <typename SampleType>
void EngineChain<SampleType>::onRelease()
{
	for (auto* node : nodes)
		node->engine->releaseResources();
}

template <typename SampleType>
typename EngineChain<SampleType>::Node& EngineChain<SampleType>::addNode (Node& newNode)
{
	jassert (! nodes.contains (&newNode));

	newNode.index = getNextNodeIndex();
	return *nodes.add (&newNode);
}

template <typename SampleType>
typename EngineChain<SampleType>::Node& EngineChain<SampleType>::addNodeToFront (Node& newNode)
{
	jassert (! nodes.contains (&newNode));

	newNode.index = 0;

	for (auto* node : nodes)
		node->index += 1;

	return *nodes.add (&newNode);
}

template <typename SampleType>
typename EngineChain<SampleType>::Node& EngineChain<SampleType>::addNodeAfter (Node& newNode, const Node& nodeToInsertAfter)
{
	jassert (nodes.contains (&nodeToInsertAfter));

	newNode.index = nodeToInsertAfter.index + 1;

	for (auto i = newNode.index; i < getNextNodeIndex(); ++i)
		if (auto* node = getNode (i))
			node->index += 1;

	return *nodes.add (&newNode);
}

template <typename SampleType>
typename EngineChain<SampleType>::Node& EngineChain<SampleType>::addNodeBefore (Node& newNode, const Node& nodeToInsertBefore)
{
	jassert (nodes.contains (&nodeToInsertBefore));

	newNode.index = std::max (0, nodeToInsertBefore.index - 1);

	for (auto i = newNode.index; i < getNextNodeIndex(); ++i)
		if (auto* node = getNode (i))
			node->index += 1;

	return *nodes.add (&newNode);
}

template <typename SampleType>
bool EngineChain<SampleType>::swapNodes (Node& node1, Node& node2)
{
	if (! nodes.contains (&node1) || ! nodes.contains (&node2))
		return false;

	const auto prevIdx1 = node1.index;
	const auto prevIdx2 = node2.index;

	jassert (prevIdx1 != prevIdx2);

	node2.index = prevIdx1;
	node1.index = prevIdx2;

	return true;
}

template <typename SampleType>
bool EngineChain<SampleType>::swapNodesAtIndices (int index1, int index2)
{
	if (index1 == index2)
		return false;

	if (auto* node1 = getNode (index1))
		if (auto* node2 = getNode (index2))
			return swapNodes (*node1, *node2);

	return false;
}

template <typename SampleType>
bool EngineChain<SampleType>::removeNode (Node& node)
{
	if (! nodes.contains (&node))
		return false;

	nodes.removeObject (&node);
	return true;
}

template <typename SampleType>
bool EngineChain<SampleType>::removeNode (int index)
{
	if (auto* node = getNode (index))
		return removeNode (*node);

	return false;
}

template class EngineChain<float>;
template class EngineChain<double>;

}  // namespace lemons::dsp
