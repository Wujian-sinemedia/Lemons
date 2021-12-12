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


/*---------------------------------------------------------------------------------------------------------------------------------*/

#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

template <typename FloatType>
LatencyEngineTests<FloatType>::LatencyEngineTests()
    : DspTest (getDspTestName<FloatType> ("Latency audio engine tests"))
{
}

template <typename FloatType>
void LatencyEngineTests<FloatType>::runTest()
{
	constexpr auto numChannels = 2;
	constexpr auto samplerate  = 44100.;

	beginTest ("Initial latency should be 0");

	expect (! engine.isInitialized());
	expectEquals (engine.reportLatency(), 0);

	for (const auto blocksize : getTestingBlockSizes())
	{
		beginTest ("Blocksize: " + String (blocksize));

		midiStorage.ensureSize (static_cast<size_t> (blocksize));
		audioIn.setSize (numChannels, blocksize, true, true, true);
		audioOut.setSize (numChannels, blocksize, true, true, true);

		fillAudioBufferWithRandomNoise (audioIn, getRandom());
		fillMidiBufferWithRandomEvents (midiStorage, blocksize / 2, getRandom());

		const auto testLatency = [&] (int latency)
		{
			engine.prepare (samplerate, blocksize, numChannels);

			expect (engine.isInitialized());

			engine.changeLatency (latency);

			expectEquals (engine.reportLatency(), latency);

			logImportantMessage ("Can call process()");

			audioOut.clear();

			engine.process (audioIn, audioOut, midiStorage);

			expect (bufferIsSilent (dsp::buffers::getAliasBuffer (audioOut, 0, std::min (latency, audioOut.getNumSamples()))));

			engine.process (audioIn, audioOut, midiStorage);
			engine.process (audioIn, audioOut, midiStorage);

			engine.releaseResources();
		};

		logImportantMessage ("Latency equal to blocksize");
		testLatency (blocksize);

		logImportantMessage ("Latency larger than blocksize");
		testLatency (blocksize * 2);

		logImportantMessage ("Latency smaller than blocksize");
		testLatency (blocksize / 2);
	}
}

template struct LatencyEngineTests<float>;
template struct LatencyEngineTests<double>;

template <typename FloatType>
void LatencyEngineTests<FloatType>::PassThroughEngine::renderChunk (const AudioBuffer<FloatType>& input,
                                                                    AudioBuffer<FloatType>&       output,
                                                                    MidiBuffer&, bool isBypassed)
{
	jassert (input.getNumSamples() == output.getNumSamples());
	jassert (input.getNumChannels() == output.getNumChannels());
	jassert (input.getNumSamples() == this->reportLatency());

	if (isBypassed)
		output.clear();
	else
		dsp::buffers::copy (input, output);
}

}  // namespace lemons::tests

#endif
