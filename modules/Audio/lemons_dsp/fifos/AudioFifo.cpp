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
AudioFifo<SampleType>::AudioFifo (int numSamples, int numChannels)
{
	resize (numSamples, numChannels);
}

template <typename SampleType>
void AudioFifo<SampleType>::pushSamples (const AudioBuffer<SampleType>& input)
{
	jassert (buffers->size() >= input.getNumChannels());

	const auto numSamples = input.getNumSamples();

	for (int chan = 0; chan < input.getNumChannels(); ++chan)
	{
		buffers[chan]->storeSamples (input.getReadPointer (chan), numSamples);
	}
}

template <typename SampleType>
void AudioFifo<SampleType>::popSamples (AudioBuffer<SampleType>& output)
{
	jassert (buffers->size() >= output.getNumChannels());

	const auto numSamples = output.getNumSamples();

	for (int chan = 0; chan < output.getNumChannels(); ++chan)
	{
		buffers[chan]->getSamples (output.getWritePointer (chan), numSamples);
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
	if (buffers->isEmpty())
		return 0;

	auto num = buffers[0]->getNumStoredSamples();

	for (const auto* buffer : buffers)
		if (buffer->getNumStoredSamples() > num)
			num = buffer->getNumStoredSamples();

	return num;
}

template <typename SampleType>
void AudioFifo<SampleType>::resize (int maxNumSamples, int numChannels)
{
	jassert (maxNumSamples > 0 && numChannels > 0);

	buffers.resize (numChannels);

	for (auto* buffer : buffers)
		buffer->resize (maxNumSamples);
}

template <typename SampleType>
int AudioFifo<SampleType>::numChannels() const noexcept
{
	return buffers->size();
}

template class AudioFifo<float>;
template class AudioFifo<double>;

}  // namespace lemons::dsp


/*---------------------------------------------------------------------------------------------------------------------------------*/


#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

template <typename FloatType>
AudioFifoTests<FloatType>::AudioFifoTests()
    : DspTest (getDspTestName<FloatType> ("Audio FIFO tests"))
{
}

template <typename FloatType>
void AudioFifoTests<FloatType>::resizeAllBuffers (int newSize, int numChannels)
{
	fifo.resize (newSize, numChannels);
	origStorage.setSize (numChannels, newSize, true, true, true);
	fifoOutput.setSize (numChannels, newSize, true, true, true);
}

template <typename FloatType>
void AudioFifoTests<FloatType>::runTest()
{
	constexpr auto numChannels = 4;

	for (const auto numSamples : getTestingBlockSizes())
	{
		beginTest ("Blocksize: " + String (numSamples));

		resizeAllBuffers (numSamples, numChannels);

		fillAudioBufferWithRandomNoise (origStorage, getRandom());

		fifo.pushSamples (origStorage);

		logImportantMessage ("Num stored samples stored correctly");

		expectEquals (fifo.numStoredSamples(), numSamples);

		fifo.popSamples (fifoOutput);

		expectEquals (fifo.numStoredSamples(), 0);

		expect (buffersAreEqual (fifoOutput, origStorage));

		const auto halfNumSamples = numSamples / 2;

		using dsp::buffers::getAliasBuffer;

		{
			const auto subtest = beginSubtest ("Retrieve fewer samples than were passed in");

			fifo.pushSamples (origStorage);

			auto outAlias = getAliasBuffer (fifoOutput, 0, halfNumSamples);

			fifo.popSamples (outAlias);

			const auto inAlias = getAliasBuffer (origStorage, 0, halfNumSamples);

			expect (buffersAreEqual (inAlias, outAlias));
		}

		{
			const auto subtest = beginSubtest ("Retrieve more samples than are left in FIFO");

			if (math::numberIsEven (numSamples))
				expectEquals (fifo.numStoredSamples(), halfNumSamples);
			else
				expectWithinAbsoluteError (fifo.numStoredSamples(), halfNumSamples, 1);

			fifo.popSamples (fifoOutput);

			for (int chan = 0; chan < numChannels; ++chan)
				expect (allSamplesAreZero (fifoOutput, 0, halfNumSamples, chan));

			const auto inAlias = getAliasBuffer (origStorage, halfNumSamples, halfNumSamples);

			auto outAlias = getAliasBuffer (fifoOutput, halfNumSamples, halfNumSamples);

			expect (buffersAreEqual (inAlias, outAlias));
		}

		{
			const auto subtest = beginSubtest ("Resizing clears the FIFO");

			fifo.pushSamples (origStorage);

			fifo.resize (halfNumSamples);

			expectEquals (fifo.numStoredSamples(), 0);
		}

		{
			const auto subtest = beginSubtest ("Increase number of channels");

			fifo.resize (halfNumSamples, numChannels + 3);

			expectEquals (fifo.numChannels(), numChannels + 3);
		}

		const auto subtest = beginSubtest ("Decrease number of channels");

		fifo.resize (halfNumSamples, numChannels);

		expectEquals (fifo.numChannels(), numChannels);
	}
}

template struct AudioFifoTests<float>;
template struct AudioFifoTests<double>;

}  // namespace lemons::tests

#endif
