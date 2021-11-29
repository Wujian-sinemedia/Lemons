
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

AudioAndMidiFifoTests::AudioAndMidiFifoTests()
    : juce::UnitTest ("AudioAndMidiFifoTests", "DSP")
{
}

void AudioAndMidiFifoTests::initialise()
{
	osc.setFrequency (440.f, 44100.);
}

void AudioAndMidiFifoTests::resizeAllBuffers (int newSize, int numChannels)
{
	origAudio.setSize (numChannels, newSize);
	audioOut.setSize (numChannels, newSize);

	const auto messages = static_cast<size_t> (newSize);
	origMidi.ensureSize (messages);
	midiOut.ensureSize (messages);

	fifo.setSize (newSize, numChannels);
}

void AudioAndMidiFifoTests::runTest()
{
	constexpr auto numSamples  = 512;
	constexpr auto numEvents   = 400;
	constexpr auto numChannels = 2;

	resizeAllBuffers (numSamples, numChannels);

	for (int chan = 0; chan < numChannels; ++chan)
		osc.getSamples (origAudio, chan);

	auto rand = getRandom();

	for (int i = 0; i < numEvents; ++i)
		origMidi.addEvent (juce::MidiMessage::controllerEvent (1, rand.nextInt (128), rand.nextInt (128)),
		                   i);

	fifo.push (origAudio, origMidi);

	beginTest ("Num stored samples stored correctly");

	expectEquals (fifo.numStoredSamples(), numSamples);

	fifo.pop (audioOut, midiOut);

	expectEquals (fifo.numStoredSamples(), 0);

	beginTest ("Store and retrieve audio and MIDI");

	expect (dsp::buffers::buffersAreEqual (audioOut, origAudio));

	expect (midi::midiBuffersAreEqual (midiOut, origMidi));

	beginTest ("Resizing clears the FIFO");

	fifo.push (origAudio, origMidi);

	fifo.setSize (numSamples / 2, numChannels + 1);

	expectEquals (fifo.numStoredSamples(), 0);
}

}  // namespace lemons::tests

#endif
