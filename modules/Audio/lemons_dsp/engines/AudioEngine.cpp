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
bool Engine<SampleType>::isInitialized() const noexcept
{
	return sampleRate > 0. && blockSize > 0;
}

template <typename SampleType>
int Engine<SampleType>::reportLatency() const noexcept
{
	return 0;
}

template <typename SampleType>
double Engine<SampleType>::getSamplerate() const noexcept
{
	return sampleRate;
}

template <typename SampleType>
int Engine<SampleType>::getBlocksize() const noexcept
{
	return blockSize;
}

template <typename SampleType>
void Engine<SampleType>::prepare (double samplerate, int blocksize, int numChannels)
{
	jassert (samplerate > 0 && blocksize > 0 && numChannels > 0);

	dummyMidiBuffer.ensureSize (static_cast<size_t> (blocksize));
	outputStorage.setSize (numChannels, blocksize, true, true, true);
	sampleRate = samplerate;
	blockSize  = blocksize;

	prepared (blocksize, samplerate, numChannels);
}

template <typename SampleType>
void Engine<SampleType>::releaseResources()
{
	dummyMidiBuffer.clear();
	outputStorage.setSize (0, 0);
	wasBypassedLastCallback = true;
	sampleRate              = 0.;
	blockSize               = 0;

	released();
}

template <typename SampleType>
int Engine<SampleType>::getNumChannels() const noexcept
{
	return outputStorage.getNumChannels();
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
	const auto numSamples = output.getNumSamples();
	jassert (numSamples == input.getNumSamples());

	if (numSamples == 0)
		return;

	if (input.getNumChannels() == 0 || output.getNumChannels() == 0)
		return;

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
void Engine<SampleType>::reset()
{
	wasBypassedLastCallback = true;
	onReset();
}

template <typename SampleType>
void Engine<SampleType>::prepared (int, double, int)
{
}


template class Engine<float>;
template class Engine<double>;


#if LEMONS_UNIT_TESTS

template <typename SampleType>
void PassThroughEngine<SampleType>::renderBlock (const AudioBuffer<SampleType>& input,
                                                 AudioBuffer<SampleType>&       output,
                                                 MidiBuffer& midiMessages, bool isBypassed)
{
	if (isBypassed)
		output.clear();
	else
		buffers::copy (input, output);
}

template class PassThroughEngine<float>;
template class PassThroughEngine<double>;

#endif

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
	constexpr auto numChannels = 3;
	constexpr auto samplerate  = 44100.;

	beginTest ("Latency should be 0");

	expect (! engine.isInitialized());
	expectEquals (engine.reportLatency(), 0);
	expectEquals (engine.getNumChannels(), 0);

	for (const auto blocksize : getTestingBlockSizes())
	{
		beginTest ("Blocksize: " + String (blocksize));

		engine.prepare (samplerate, blocksize, numChannels);
		midiStorage.ensureSize (static_cast<size_t> (blocksize));
		audioIn.setSize (numChannels, blocksize, true, true, true);
		audioOut.setSize (numChannels, blocksize, true, true, true);

		expect (engine.isInitialized());
		expectEquals (engine.getSamplerate(), samplerate);
		expectEquals (engine.getNumChannels(), numChannels);

		fillAudioBufferWithRandomNoise (audioIn, getRandom());
		fillMidiBufferWithRandomEvents (midiStorage, blocksize / 2, getRandom());

		logImportantMessage ("Can call process()");

		engine.process (audioIn);

		expect (allSamplesAreValid (audioIn));

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
		expectEquals (engine.getSamplerate(), 0.);
		expectEquals (engine.getNumChannels(), 0);
	}
}

template struct AudioEngineTests<float>;
template struct AudioEngineTests<double>;

}  // namespace lemons::tests

#endif
