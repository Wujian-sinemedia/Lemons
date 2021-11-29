#include <lemons_core/lemons_core.h>

namespace lemons::dsp
{

template <typename SampleType>
CircularBuffer<SampleType>::CircularBuffer (int initialCapacity)
    : buffer (1, initialCapacity)
    , fifo (initialCapacity)
{
}

template <typename SampleType>
void CircularBuffer<SampleType>::storeSamples (const AudioBuffer<SampleType>& samples, int channel)
{
	jassert (channel >= 0);
	storeSamples (samples.getReadPointer (channel), samples.getNumSamples());
}

template <typename SampleType>
void CircularBuffer<SampleType>::storeSamples (const SampleType* samples, int numSamples)
{
	jassert (buffer.getNumSamples() == fifo.getTotalSize());

	const auto scopedWrite = fifo.write (numSamples);

	// the buffer isn't big enough to hold all the samples you want to store!
	jassert (scopedWrite.blockSize1 + scopedWrite.blockSize2 == numSamples);

	if (scopedWrite.blockSize1 > 0)
	{
		vecops::copy (samples,
		              buffer.getWritePointer (0, scopedWrite.startIndex1),
		              scopedWrite.blockSize1);
	}

	if (scopedWrite.blockSize2 > 0)
	{
		vecops::copy (samples + scopedWrite.blockSize1,
		              buffer.getWritePointer (0, scopedWrite.startIndex2),
		              scopedWrite.blockSize2);
	}
}

template <typename SampleType>
void CircularBuffer<SampleType>::getSamples (AudioBuffer<SampleType>& output, int channel)
{
	jassert (channel >= 0);
	getSamples (output.getWritePointer (channel), output.getNumSamples());
}

template <typename SampleType>
void CircularBuffer<SampleType>::getSamples (SampleType* output, int numSamples)
{
	jassert (buffer.getNumSamples() == fifo.getTotalSize());

	const auto scopedRead = fifo.read (numSamples);

	const auto numZeroes = numSamples - (scopedRead.blockSize1 + scopedRead.blockSize2);

	if (numZeroes > 0)
		vecops::fill (output, SampleType (0), numZeroes);

	auto* const sampleOutput = output + numZeroes;

	if (scopedRead.blockSize1 > 0)
	{
		vecops::copy (buffer.getReadPointer (0, scopedRead.startIndex1),
		              sampleOutput,
		              scopedRead.blockSize1);
	}

	if (scopedRead.blockSize2 > 0)
	{
		vecops::copy (buffer.getReadPointer (0, scopedRead.startIndex2),
		              sampleOutput + scopedRead.blockSize1,
		              scopedRead.blockSize2);
	}
}

template <typename SampleType>
void CircularBuffer<SampleType>::resize (int newSize)
{
	jassert (newSize > 0);

	clear();

	// NB. avoids edge cases when attempting to store the full capacity's worth of samples
	newSize += 1;

	buffer.setSize (1, newSize, true, true, false);

	fifo.setTotalSize (newSize);

	jassert (buffer.getNumSamples() == fifo.getTotalSize());
}

template <typename SampleType>
int CircularBuffer<SampleType>::getCapacity() const noexcept
{
	jassert (buffer.getNumSamples() == fifo.getTotalSize());
	return buffer.getNumSamples() - 1;
}

template <typename SampleType>
int CircularBuffer<SampleType>::getNumStoredSamples() const noexcept
{
	jassert (buffer.getNumSamples() == fifo.getTotalSize());
	return fifo.getNumReady();
}

template <typename SampleType>
void CircularBuffer<SampleType>::clear()
{
	buffer.clear();
	fifo.reset();
}


template class CircularBuffer<float>;
template class CircularBuffer<double>;

}  // namespace lemons::dsp


/*---------------------------------------------------------------------------------------------------------------------------------*/


#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

template <typename FloatType>
CircularBufferTests<FloatType>::CircularBufferTests()
    : DspTest ("CircularBufferTests")
{
}

template <typename FloatType>
void CircularBufferTests<FloatType>::initialise()
{
	osc.setFrequency (440.f, 44100.);
}

template <typename FloatType>
void CircularBufferTests<FloatType>::resizeAllBuffers (int newSize)
{
	circularBuffer.resize (newSize);
	origStorage.setSize (1, newSize, true, true, true);
	circOutput.setSize (1, newSize, true, true, true);
}

template <typename FloatType>
void CircularBufferTests<FloatType>::runTest()
{
	for (const auto numSamples : getTestingBlockSizes())
	{
		logBlocksizeMessage (numSamples);

		resizeAllBuffers (numSamples);

		osc.getSamples (origStorage);

		circularBuffer.storeSamples (origStorage);


		beginTest ("Num stored samples stored correctly");

		expectEquals (circularBuffer.getNumStoredSamples(), numSamples);

		circularBuffer.getSamples (circOutput);

		expectEquals (circularBuffer.getNumStoredSamples(), 0);


		beginTest ("Store samples and retrieve later");

		expect (allSamplesAreEqual (circOutput, origStorage, 0, numSamples));


		beginTest ("Retrieve fewer samples than were passed in");

		circularBuffer.storeSamples (origStorage);

		const auto halfNumSamples = numSamples / 2;

		auto alias = dsp::buffers::getAliasBuffer (circOutput, 0, halfNumSamples);

		circularBuffer.getSamples (alias);

		expect (allSamplesAreEqual (alias, origStorage, 0, halfNumSamples));


		beginTest ("Retrieve more samples than are left in circ buffer");

		expectEquals (circularBuffer.getNumStoredSamples(), halfNumSamples);

		circularBuffer.getSamples (circOutput);

		expect (allSamplesAreZero (circOutput, 0, halfNumSamples));

		expect (allSamplesAreEqual (circOutput, origStorage, halfNumSamples, halfNumSamples));


		beginTest ("Resizing");

		circularBuffer.storeSamples (origStorage);

		circularBuffer.resize (halfNumSamples);

		expectEquals (circularBuffer.getCapacity(), halfNumSamples);


		beginTest ("Resizing clears the buffer");

		expectEquals (circularBuffer.getNumStoredSamples(), 0);
	}
}

template struct CircularBufferTests<float>;
template struct CircularBufferTests<double>;

}  // namespace lemons::tests

#endif
