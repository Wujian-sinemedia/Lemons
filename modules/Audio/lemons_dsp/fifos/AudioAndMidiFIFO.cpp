
namespace lemons::dsp
{
template <typename SampleType>
AudioAndMidiFIFO<SampleType>::AudioAndMidiFIFO (int samples)
{
	setSize (samples);
}

template <typename SampleType>
void AudioAndMidiFIFO<SampleType>::setSize (int numSamples, int numChannels)
{
	jassert (numSamples > 0 && numChannels > 0);

	clear();

	audio.resize (numSamples, numChannels);
	midi.setSize (numSamples);
}

template <typename SampleType>
void AudioAndMidiFIFO<SampleType>::push (const AudioBuffer<SampleType>& audioIn, const MidiBuffer& midiIn)
{
	audio.pushSamples (audioIn);
	midi.pushEvents (midiIn, audioIn.getNumSamples());

	jassert (audio.numStoredSamples() == midi.numStoredSamples());
}

template <typename SampleType>
void AudioAndMidiFIFO<SampleType>::pop (AudioBuffer<SampleType>& audioOut, MidiBuffer& midiOut)
{
	audio.popSamples (audioOut);
	midi.popEvents (midiOut, audioOut.getNumSamples());

	jassert (audio.numStoredSamples() == midi.numStoredSamples());
}

template <typename SampleType>
int AudioAndMidiFIFO<SampleType>::numStoredSamples() const noexcept
{
	jassert (audio.numStoredSamples() == midi.numStoredSamples());
	return audio.numStoredSamples();
}

template <typename SampleType>
void AudioAndMidiFIFO<SampleType>::clear()
{
	audio.clear();
	midi.clear();
}

template class AudioAndMidiFIFO<float>;
template class AudioAndMidiFIFO<double>;

}  // namespace lemons::dsp


/*---------------------------------------------------------------------------------------------------------------------------------*/


#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

template <typename FloatType>
AudioAndMidiFifoTests<FloatType>::AudioAndMidiFifoTests()
    : DspTest (getDspTestName<FloatType> ("Audio and MIDI FIFO tests"))
{
}

template <typename FloatType>
void AudioAndMidiFifoTests<FloatType>::resizeAllBuffers (int newSize, int numChannels)
{
	origAudio.setSize (numChannels, newSize, true, true, true);
	audioOut.setSize (numChannels, newSize, true, true, true);

	const auto messages = static_cast<size_t> (newSize);
	origMidi.ensureSize (messages);
	midiOut.ensureSize (messages);

	fifo.setSize (newSize, numChannels);
}

template <typename FloatType>
void AudioAndMidiFifoTests<FloatType>::runTest()
{
	constexpr auto numChannels = 2;

	for (const auto numSamples : getTestingBlockSizes())
	{
		beginTest ("Blocksize: " + String (numSamples));

		resizeAllBuffers (numSamples, numChannels);

		fillAudioBufferWithRandomNoise (origAudio);
		fillMidiBufferWithRandomEvents (origMidi, numSamples / 3);

		fifo.push (origAudio, origMidi);


		logImportantMessage ("Num stored samples stored correctly");

		expectEquals (fifo.numStoredSamples(), numSamples);

		fifo.pop (audioOut, midiOut);

		expectEquals (fifo.numStoredSamples(), 0);


		logImportantMessage ("Store and retrieve audio and MIDI");

		expect (buffersAreEqual (audioOut, origAudio));
		expect (midiBuffersAreEqual (midiOut, origMidi));


		logImportantMessage ("Resizing clears the FIFO");

		fifo.push (origAudio, origMidi);

		fifo.setSize (numSamples / 2, numChannels + 1);

		expectEquals (fifo.numStoredSamples(), 0);
	}
}

template struct AudioAndMidiFifoTests<float>;
template struct AudioAndMidiFifoTests<double>;

}  // namespace lemons::tests

#endif
