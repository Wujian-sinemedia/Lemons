/*
 ======================================================================================

 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.

 ======================================================================================
 */


namespace lemons::dsp
{

template <typename SampleType>
int LatencyEngine<SampleType>::reportLatency() const noexcept
{
	return internalBlocksize;
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
void LatencyEngine<SampleType>::changeLatency (int newInternalBlocksize, bool setTopLevelEngineBlocksize)
{
	jassert (newInternalBlocksize > 0);
	jassert (this->isInitialized());

	internalBlocksize = newInternalBlocksize;

	const auto multiple = newInternalBlocksize * 4;

	const auto channels = this->getNumChannels();
	jassert (channels > 0);

	inputFIFO.setSize (multiple, channels);
	outputFIFO.setSize (multiple, channels);

	chunkMidiBuffer.ensureSize (static_cast<size_t> (newInternalBlocksize));
	chunkMidiBuffer.clear();

	inBuffer.setSize (channels, multiple, true, true, true);
	outBuffer.setSize (channels, multiple, true, true, true);

	inBuffer.clear();
	outBuffer.clear();

	// push newInternalBlocksize worth of silent samples into outputFIFO
	outputFIFO.push (buffers::getAliasBuffer (outBuffer, 0, newInternalBlocksize),
	                 chunkMidiBuffer);

	latencyChanged (newInternalBlocksize);

	if (setTopLevelEngineBlocksize)
	{
		const auto samplerate = this->getSamplerate();
		jassert (samplerate > 0.);

		this->prepare (samplerate, internalBlocksize, channels);
	}
}

template <typename SampleType>
void LatencyEngine<SampleType>::renderBlock (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midiMessages, bool isBypassed)
{
	jassert (internalBlocksize > 0);

	const auto totalNumSamples = input.getNumSamples();
	jassert (totalNumSamples == output.getNumSamples());

	inputFIFO.push (input, midiMessages);

	while (inputFIFO.numStoredSamples() >= internalBlocksize)
	{
		auto inAlias  = buffers::getAliasBuffer (inBuffer, 0, internalBlocksize);
		auto outAlias = buffers::getAliasBuffer (outBuffer, 0, internalBlocksize);

		inputFIFO.pop (inAlias, chunkMidiBuffer);

		outAlias.clear();

		renderChunk (inAlias, outAlias, chunkMidiBuffer, isBypassed);

		outputFIFO.push (outAlias, chunkMidiBuffer);
	}

	outputFIFO.pop (output, midiMessages);
}

template <typename SampleType>
void LatencyEngine<SampleType>::latencyChanged (int)
{
}

template class LatencyEngine<float>;
template class LatencyEngine<double>;

}  // namespace lemons::dsp

