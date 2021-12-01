namespace lemons::dsp
{

template <typename SampleType>
void Engine<SampleType>::prepare (double samplerate, int blocksize, int numChannels)
{
	jassert (samplerate > 0 && blocksize > 0 && numChannels > 0);

	dummyMidiBuffer.ensureSize (static_cast<size_t> (blocksize));
	outputStorage.setSize (numChannels, blocksize);
	sampleRate = samplerate;

	prepared (blocksize, samplerate, numChannels);
}

template <typename SampleType>
void Engine<SampleType>::releaseResources()
{
	dummyMidiBuffer.clear();
	wasBypassedLastCallback = true;
	sampleRate              = 0.;
	released();
}

template <typename SampleType>
void Engine<SampleType>::process (AudioBuffer<SampleType>& inplaceInAndOut,
                                  MidiBuffer&              midiMessages,
                                  bool                     isBypassed)
{
	processInternal (inplaceInAndOut, inplaceInAndOut, midiMessages, isBypassed);
}

template <typename SampleType>
void Engine<SampleType>::process (const AudioBuffer<SampleType>& input,
                                  AudioBuffer<SampleType>&       output,
                                  MidiBuffer&                    midiMessages,
                                  bool                           isBypassed)
{
	processInternal (input, output, midiMessages, isBypassed);
}

template <typename SampleType>
void Engine<SampleType>::process (AudioBuffer<SampleType>& inplaceInAndOut,
                                  bool                     isBypassed)
{
	dummyMidiBuffer.clear();
	processInternal (inplaceInAndOut, inplaceInAndOut, dummyMidiBuffer, isBypassed);
}

template <typename SampleType>
void Engine<SampleType>::process (const AudioBuffer<SampleType>& input,
                                  AudioBuffer<SampleType>&       output,
                                  bool                           isBypassed)
{
	dummyMidiBuffer.clear();
	processInternal (input, output, dummyMidiBuffer, isBypassed);
}

template <typename SampleType>
void Engine<SampleType>::processInternal (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midiMessages, bool isBypassed)
{
	jassert (isInitialized());

	bool applyFadeIn                 = false;
	bool applyFadeOut                = false;
	bool processingBypassedThisFrame = isBypassed;

	if (isBypassed && ! wasBypassedLastCallback)
	{
		applyFadeOut                = true;
		processingBypassedThisFrame = false;
	}
	else if (wasBypassedLastCallback && ! isBypassed)
	{
		applyFadeIn                 = true;
		processingBypassedThisFrame = false;
	}

	jassert (! (applyFadeIn && applyFadeOut));

	const auto numSamples = output.getNumSamples();

	auto alias = buffers::getAliasBuffer (outputStorage, 0, numSamples);

	alias.clear();

	renderBlock (input, alias, midiMessages, processingBypassedThisFrame);

	if (applyFadeIn)
		alias.applyGainRamp (0, numSamples, SampleType (0), SampleType (1));
	else if (applyFadeOut)
		alias.applyGainRamp (0, numSamples, SampleType (1), SampleType (0));

	buffers::copy (alias, output);

	wasBypassedLastCallback = isBypassed;
}

template <typename SampleType>
void Engine<SampleType>::prepared (int, double, int)
{
}


template class Engine<float>;
template class Engine<double>;

}  // namespace lemons::dsp


/*---------------------------------------------------------------------------------------------------------------------------------*/


#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

template <typename FloatType>
AudioEngineTests<FloatType>::AudioEngineTests()
    : DspTest (getDspTestName<FloatType> ("Audio engine tests"))
{
}

template <typename FloatType>
void AudioEngineTests<FloatType>::runTest()
{
	constexpr auto numChannels = 2;
	constexpr auto samplerate  = 44100.;

	beginTest ("Latency should be 0");

	expect (! engine.isInitialized());
	expectEquals (engine.reportLatency(), 0);

	for (const auto blocksize : getTestingBlockSizes())
	{
		beginTest ("Blocksize: " + String (blocksize));

		engine.prepare (samplerate, blocksize, numChannels);
		midiStorage.ensureSize (static_cast<size_t> (blocksize));
		audioIn.setSize (numChannels, blocksize, true, true, true);
		audioOut.setSize (numChannels, blocksize, true, true, true);

		expect (engine.isInitialized());
		expectEquals (engine.getSamplerate(), samplerate);

		fillAudioBufferWithRandomNoise (audioIn);
		fillMidiBufferWithRandomEvents (midiStorage, blocksize / 2);

		logImportantMessage ("Can call process()");

		engine.process (audioIn);
		engine.process (audioIn, midiStorage);

		logImportantMessage ("Alternate bypassed/unbypassed processing");

		audioOut.clear();

		engine.process (audioIn, audioOut, false);
		expect (buffersAreEqual (audioIn, audioOut));

		engine.process (audioIn, audioOut, true);
		expect (! bufferIsSilent (audioOut));
		expect (! buffersAreEqual (audioIn, audioOut));

		engine.process (audioIn, audioOut, true);
		expect (bufferIsSilent (audioOut));

		engine.process (audioIn, audioOut, true);
		expect (bufferIsSilent (audioOut));

		engine.process (audioIn, audioOut, false);
		expect (! bufferIsSilent (audioOut));
		expect (! buffersAreEqual (audioIn, audioOut));

		engine.process (audioIn, audioOut, false);
		expect (buffersAreEqual (audioIn, audioOut));

		engine.process (audioIn, true);
		engine.process (audioIn, false);
		engine.process (audioIn, true);
		engine.process (audioIn, false);
		engine.process (audioIn, true);

		logImportantMessage ("Release");

		engine.releaseResources();

		expect (! engine.isInitialized());
	}
}

template <typename FloatType>
void AudioEngineTests<FloatType>::PassThroughEngine::renderBlock (const AudioBuffer<FloatType>& input,
                                                                  AudioBuffer<FloatType>&       output,
                                                                  MidiBuffer&, bool bypassed)
{
	if (! bypassed)
		dsp::buffers::copy (input, output);
}

template struct AudioEngineTests<float>;
template struct AudioEngineTests<double>;

}  // namespace lemons::tests

#endif
