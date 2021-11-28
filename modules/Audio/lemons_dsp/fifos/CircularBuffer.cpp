#include <lemons_core/lemons_core.h>

namespace lemons::dsp
{

template <typename SampleType>
void CircularBuffer<SampleType>::resize (int newSize)
{
	buffer.setSize (1, newSize, true, true, true);

	lastFrameStart %= newSize;
	lastFrameEnd %= newSize;
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
	const auto bufferSize = buffer.getNumSamples();

	jassert (numSamples > 0 && numSamples <= bufferSize);

	const auto newStart = (lastFrameEnd + 1) % bufferSize;

	jassert (newStart >= 0 && newStart < bufferSize);

	if (newStart + numSamples > bufferSize)
	{
		const auto chunkAtEndOfBuffer = bufferSize - newStart;

		vecops::copy (samples,
		              buffer.getWritePointer (0, newStart),
		              chunkAtEndOfBuffer);

		const auto left = numSamples - chunkAtEndOfBuffer;

		if (left > 0)
			vecops::copy (samples + chunkAtEndOfBuffer,
			              buffer.getWritePointer (0, 0),
			              left);

		lastFrameEnd = left;
	}
	else
	{
		vecops::copy (samples,
		              buffer.getWritePointer (0, newStart),
		              numSamples);

		lastFrameEnd = newStart + numSamples;
	}

	lastFrameStart = newStart;
	numStoredSamples += numSamples;
}

template <typename SampleType>
void CircularBuffer<SampleType>::getSamples (AudioBuffer<SampleType>& output, int channel)
{
	jassert (channel >= 0);
	getSamples (output.getWritePointer (channel), output.getNumSamples());
}

inline int getNumZeroesToOutput (int totalNumSamplesWanted, int numStoredSamples)
{
	if (totalNumSamplesWanted <= numStoredSamples)
		return 0;

	return totalNumSamplesWanted - numStoredSamples;
}

template <typename SampleType>
void CircularBuffer<SampleType>::getSamples (SampleType* output, int numSamples)
{
	const auto bufferSize = buffer.getNumSamples();

	jassert (numSamples > 0 && numSamples <= bufferSize);

	const auto zeroes  = getNumZeroesToOutput (numSamples, numStoredSamples);
	const auto samples = numSamples - zeroes;

	if (zeroes > 0)
		vecops::fill (output, SampleType (0), zeroes);

	const auto startIdx = lastFrameEnd - samples;

	auto* sampleOutput = output + zeroes;

	if (startIdx < 0)
	{
		const auto endChunkIdx        = bufferSize + startIdx;
		const auto chunkAtEndOfBuffer = bufferSize - endChunkIdx;

		vecops::copy (buffer.getReadPointer (0, endChunkIdx),
		              sampleOutput,
		              chunkAtEndOfBuffer);

		const auto left = samples - chunkAtEndOfBuffer;

		if (left > 0)
			vecops::copy (buffer.getReadPointer (0, 0),
			              sampleOutput + chunkAtEndOfBuffer,
			              left);
	}
	else
	{
		vecops::copy (buffer.getReadPointer (0, startIdx),
		              sampleOutput,
		              samples);
	}

	numStoredSamples = std::max (0, numStoredSamples - samples);
}

template <typename SampleType>
int CircularBuffer<SampleType>::getCapacity() const noexcept
{
	return buffer.getNumSamples();
}

template <typename SampleType>
int CircularBuffer<SampleType>::getNumStoredSamples() const noexcept
{
	return numStoredSamples;
}

template <typename SampleType>
void CircularBuffer<SampleType>::clear()
{
	lastFrameStart   = 0;
	lastFrameEnd     = 0;
	numStoredSamples = 0;

	buffer.clear();
}


template class CircularBuffer<float>;
template class CircularBuffer<double>;

}  // namespace lemons::dsp
