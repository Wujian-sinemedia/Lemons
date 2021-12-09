namespace lemons::dsp
{

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
	for (auto* node : nodes)
		if (node->index == index)
			return node;

	return nullptr;
}

template <typename SampleType>
int EngineChain<SampleType>::getNextNodeIndex() const
{
	int idx = -1;

	for (auto* node : nodes)
		if (node->index > idx)
			idx = node->index;

	return idx + 1;
}

template <typename SampleType>
void EngineChain<SampleType>::renderChunk (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midiMessages, bool isBypassed)
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
	{
		const auto l = node->engine->reportLatency();

		if (l > latency)
			latency = l;
	}

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

template class EngineChain<float>;
template class EngineChain<double>;

}  // namespace lemons::dsp
