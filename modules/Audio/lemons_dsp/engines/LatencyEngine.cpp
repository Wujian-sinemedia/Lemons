namespace lemons::dsp
{

template <typename SampleType>
void LatencyEngine<SampleType>::prepared (int blocksize, double samplerate, int numChannels)
{
	jassert (blocksize == internalBlocksize);

	chunkMidiBuffer.ensureSize (static_cast<size_t> (blocksize));
	inputFIFO.setSize (blocksize, numChannels);
	outputFIFO.setSize (blocksize, numChannels);
	inBuffer.setSize (numChannels, blocksize, true, true, true);
	outBuffer.setSize (numChannels, blocksize, true, true, true);

	onPrepare (blocksize, samplerate);
}

template <typename SampleType>
void LatencyEngine<SampleType>::released()
{
	chunkMidiBuffer.clear();
	inputFIFO.clear();
	outputFIFO.clear();
	inBuffer.setSize (0, 0);
	outBuffer.setSize (0, 0);
	internalBlocksize = 0;

	onRelease();
}

template <typename SampleType>
void LatencyEngine<SampleType>::changeLatency (int newInternalBlocksize)
{
	jassert (newInternalBlocksize > 0);

	internalBlocksize = newInternalBlocksize;

	auto channels = inBuffer.getNumChannels();
	if (channels == 0) channels = 2;

	auto samplerate = this->getSamplerate();
	if (samplerate == 0.) samplerate = 44100.;

	this->prepare (samplerate, internalBlocksize, channels);
}

template <typename SampleType>
void LatencyEngine<SampleType>::setSamplerate (double samplerate)
{
	jassert (samplerate > 0.);

	auto channels = inBuffer.getNumChannels();
	if (channels == 0) channels = 2;

	this->prepare (samplerate, internalBlocksize, channels);
}

template <typename SampleType>
void LatencyEngine<SampleType>::renderBlock (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midiMessages, bool isBypassed)
{
	jassert (internalBlocksize > 0);

	if (input.getNumChannels() == 0 || output.getNumChannels() == 0)
		return;

	const auto totalNumSamples = input.getNumSamples();

	jassert (totalNumSamples == output.getNumSamples());

	if (totalNumSamples == 0)
		return;

	inputFIFO.push (input, midiMessages);

	while (inputFIFO.numStoredSamples() >= internalBlocksize)
	{
		auto inAlias = buffers::getAliasBuffer (inBuffer, 0, internalBlocksize);

		auto outAlias = buffers::getAliasBuffer (outBuffer, 0, internalBlocksize);

		inputFIFO.pop (inAlias, chunkMidiBuffer);

		renderChunk (inAlias, outAlias, chunkMidiBuffer, isBypassed);

		outputFIFO.push (outAlias, chunkMidiBuffer);
	}

	outputFIFO.pop (output, midiMessages);
}

template <typename SampleType>
void LatencyEngine<SampleType>::onPrepare (int, double)
{
}

template class LatencyEngine<float>;
template class LatencyEngine<double>;

}  // namespace lemons::dsp
