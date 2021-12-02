namespace lemons::dsp
{

template <typename SampleType>
void LatencyEngine<SampleType>::prepared (int blocksize, double samplerate, int numChannels)
{
	jassert (blocksize == internalBlocksize);

	const auto multiple = blocksize * 4;

	inputFIFO.setSize (multiple, numChannels);
	outputFIFO.setSize (multiple, numChannels);

	chunkMidiBuffer.ensureSize (static_cast<size_t> (blocksize));

	inBuffer.setSize (numChannels, multiple, true, true, true);
	outBuffer.setSize (numChannels, multiple, true, true, true);

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

	if (internalBlocksize == 0) internalBlocksize = 512;

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
		auto inAlias  = buffers::getAliasBuffer (inBuffer, 0, internalBlocksize);
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

		engine.setSamplerate (samplerate);
		expectEquals (engine.getSamplerate(), samplerate);

		expect (engine.isInitialized());

		logImportantMessage ("Latency equal to blocksize");
		testLatency (blocksize);

		logImportantMessage ("Latency larger than blocksize");
		testLatency (blocksize * 2 + 36);

		logImportantMessage ("Latency smaller than blocksize");
		testLatency (blocksize / 2);
	}
}

template <typename FloatType>
void LatencyEngineTests<FloatType>::testLatency (int latency)
{
	{
		const auto srBefore = engine.getSamplerate();

		engine.changeLatency (latency);
		expectEquals (engine.reportLatency(), latency);
		expectEquals (engine.getSamplerate(), srBefore);
	}

	//    logImportantMessage ("Can call process()");

	audioOut.clear();

	// engine.process (audioIn, audioOut, midiStorage);

	// expect (allSamplesAreZero (audioOut, 0, latency));
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

	if (! isBypassed)
		dsp::buffers::copy (input, output);
}

}  // namespace lemons::tests

#endif
