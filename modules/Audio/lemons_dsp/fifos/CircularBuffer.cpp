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
	jassert (channel >= 0 && channel < samples.getNumChannels());
	storeSamples (samples.getReadPointer (channel), samples.getNumSamples());
}

template <typename SampleType>
void CircularBuffer<SampleType>::storeSamples (const SampleType* samples, int numSamples)
{
	jassert (buffer.getNumSamples() == fifo.getTotalSize());

	const auto scopedWrite = fifo.write (numSamples);

	// the buffer isn't big enough to hold all the samples you want to store!
	jassert (scopedWrite.blockSize1 + scopedWrite.blockSize2 == numSamples);

	using FVO = juce::FloatVectorOperations;

	if (scopedWrite.blockSize1 > 0)
	{
		FVO::copy (buffer.getWritePointer (0, scopedWrite.startIndex1),
		           samples,
		           scopedWrite.blockSize1);
	}

	if (scopedWrite.blockSize2 > 0)
	{
		FVO::copy (buffer.getWritePointer (0, scopedWrite.startIndex2),
		           samples + scopedWrite.blockSize1,
		           scopedWrite.blockSize2);
	}
}

template <typename SampleType>
void CircularBuffer<SampleType>::getSamples (AudioBuffer<SampleType>& output, int channel)
{
	jassert (channel >= 0 && channel < output.getNumChannels());
	getSamples (output.getWritePointer (channel), output.getNumSamples());
}

template <typename SampleType>
void CircularBuffer<SampleType>::getSamples (SampleType* output, int numSamples)
{
	jassert (buffer.getNumSamples() == fifo.getTotalSize());

	const auto scopedRead = fifo.read (numSamples);

	const auto numZeroes = numSamples - (scopedRead.blockSize1 + scopedRead.blockSize2);

	if (numZeroes > 0)
		juce::FloatVectorOperations::fill (output, SampleType (0), numZeroes);

	auto* const sampleOutput = output + numZeroes;

	using FVO = juce::FloatVectorOperations;

	if (scopedRead.blockSize1 > 0)
	{
		FVO::copy (sampleOutput,
		           buffer.getReadPointer (0, scopedRead.startIndex1),
		           scopedRead.blockSize1);
	}

	if (scopedRead.blockSize2 > 0)
	{
		FVO::copy (sampleOutput + scopedRead.blockSize1,
		           buffer.getReadPointer (0, scopedRead.startIndex2),
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

	buffer.setSize (1, newSize, true, true, true);

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

