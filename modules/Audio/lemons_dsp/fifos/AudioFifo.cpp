namespace lemons::dsp
{
template <typename SampleType>
AudioFifo<SampleType>::AudioFifo (int numSamples, int numChannels)
{
	resize (numSamples, numChannels);
}

template <typename SampleType>
void AudioFifo<SampleType>::pushSamples (const AudioBuffer<SampleType>& input)
{
	jassert (buffers.size() >= input.getNumChannels());

	const auto numSamples = input.getNumSamples();

	for (int chan = 0; chan < input.getNumChannels(); ++chan)
	{
		buffers.getUnchecked (chan)->storeSamples (input.getReadPointer (chan),
		                                           numSamples);
	}
}

template <typename SampleType>
void AudioFifo<SampleType>::popSamples (AudioBuffer<SampleType>& output)
{
	jassert (buffers.size() >= output.getNumChannels());

	const auto numSamples = output.getNumSamples();

	for (int chan = 0; chan < output.getNumChannels(); ++chan)
	{
		buffers.getUnchecked (chan)->getSamples (output.getWritePointer (chan),
		                                         numSamples);
	}
}

template <typename SampleType>
void AudioFifo<SampleType>::clear()
{
	for (auto* buffer : buffers)
		buffer->clear();
}

template <typename SampleType>
int AudioFifo<SampleType>::numStoredSamples() const noexcept
{
	if (buffers.isEmpty())
		return 0;

	auto num = buffers.getUnchecked (0)->getNumStoredSamples();

	for (auto* buffer : buffers)
		if (buffer->getNumStoredSamples() < num)
			num = buffer->getNumStoredSamples();

	return num;
}

template <typename SampleType>
void AudioFifo<SampleType>::resize (int maxNumSamples, int numChannels)
{
	jassert (maxNumSamples > 0 && numChannels > 0);

	while (buffers.size() < numChannels)
		buffers.add (new CircularBuffer<SampleType>());

	while (buffers.size() > numChannels)
		buffers.removeLast();

	for (auto* buffer : buffers)
		buffer->resize (maxNumSamples);
}

template <typename SampleType>
int AudioFifo<SampleType>::numChannels() const noexcept
{
	return buffers.size();
}

template class AudioFifo<float>;
template class AudioFifo<double>;

}  // namespace lemons::dsp


/*---------------------------------------------------------------------------------------------------------------------------------*/


#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

template<typename FloatType>
AudioFifoTests<FloatType>::AudioFifoTests()
    : juce::UnitTest ("AudioFifoTests", "DSP")
{
}

template<typename FloatType>
void AudioFifoTests<FloatType>::initialise()
{
	osc.setFrequency (440.f, 44100.);
}

template<typename FloatType>
void AudioFifoTests<FloatType>::resizeAllBuffers (int newSize, int numChannels)
{
	fifo.resize (newSize, numChannels);
	origStorage.setSize (numChannels, newSize, true, true, true);
	fifoOutput.setSize (numChannels, newSize, true, true, true);
}

template<typename FloatType>
void AudioFifoTests<FloatType>::runTest()
{
	constexpr auto numSamples  = 44100;
	constexpr auto numChannels = 4;

	resizeAllBuffers (numSamples, numChannels);

	for (int chan = 0; chan < numChannels; ++chan)
		osc.getSamples (origStorage, chan);

	fifo.pushSamples (origStorage);

    
	beginTest ("Num stored samples stored correctly");

	expectEquals (fifo.numStoredSamples(), numSamples);

	fifo.popSamples (fifoOutput);

	expectEquals (fifo.numStoredSamples(), 0);

    
	beginTest ("Store samples and retrieve later");

	using namespace dsp::buffers;

	expect (buffersAreEqual (fifoOutput, origStorage));

    
	beginTest ("Retrieve fewer samples than were passed in");

	fifo.pushSamples (origStorage);

	const auto halfNumSamples = numSamples / 2;

	{
		auto outAlias = getAliasBuffer (fifoOutput, 0, halfNumSamples);

		fifo.popSamples (outAlias);

		const auto inAlias = getAliasBuffer (origStorage, 0, halfNumSamples);

		expect (buffersAreEqual (inAlias, outAlias));
	}

    
	beginTest ("Retrieve more samples than are left in FIFO");

	expectEquals (fifo.numStoredSamples(), halfNumSamples);

	fifo.popSamples (fifoOutput);

	for (int chan = 0; chan < numChannels; ++chan)
		expect (allSamplesAreZero (fifoOutput, 0, halfNumSamples, chan));


	{
		const auto inAlias = getAliasBuffer (origStorage, halfNumSamples, halfNumSamples);

		auto outAlias = getAliasBuffer (fifoOutput, halfNumSamples, halfNumSamples);

		expect (buffersAreEqual (inAlias, outAlias));
	}

    
	beginTest ("Resizing clears the FIFO");

	fifo.pushSamples (origStorage);

	fifo.resize (halfNumSamples);

	expectEquals (fifo.numStoredSamples(), 0);

    
	beginTest ("Increase number of channels");

	fifo.resize (halfNumSamples, numChannels + 3);

	expectEquals (fifo.numChannels(), numChannels + 3);

    
	beginTest ("Decrease number of channels");

	fifo.resize (halfNumSamples, numChannels);

	expectEquals (fifo.numChannels(), numChannels);
}

template struct AudioFifoTests<float>;
template struct AudioFifoTests<double>;

}  // namespace lemons::tests

#endif
