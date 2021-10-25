
namespace lemons::dsp
{
template <typename SampleType>
void AudioFIFO<SampleType>::pushSamples (const SampleType* samples, int numSamples)
{
	buffer.storeSamples (samples, numSamples);
	numStored += numSamples;
}

static inline int getZeroesToOutput (int totalNumSamplesWanted, int numStoredSamples)
{
	if (totalNumSamplesWanted <= numStoredSamples)
		return 0;

	return totalNumSamplesWanted - numStoredSamples;
}

template <typename SampleType>
void AudioFIFO<SampleType>::popSamples (SampleType* output, int numSamples)
{
	const auto zeroes  = getZeroesToOutput (numSamples, numStored);
	const auto samples = numSamples - zeroes;

	for (int i = 0; i < zeroes; ++i)
		output[i] = SampleType (0);

	const auto bufferCapacity = buffer.getCapacity();

	auto index = buffer.getLastFrameEndIndex() - samples;
	if (index < 0) index += bufferCapacity;

	for (int i = 0; i < samples; ++i)
	{
		output[i + zeroes] = buffer.getSample (index++);
		if (index >= bufferCapacity) index = 0;
	}

	numStored = std::max (0, numStored - samples);
}

template <typename SampleType>
int AudioFIFO<SampleType>::numStoredSamples() const
{
	return numStored;
}

template <typename SampleType>
void AudioFIFO<SampleType>::setMaximumSize (int maximumCapacitySamples)
{
	buffer.resize (maximumCapacitySamples, 4);
}

template class AudioFIFO<float>;
template class AudioFIFO<double>;


template <typename SampleType, size_t numChannels>
MultiAudioFIFO<SampleType, numChannels>::MultiAudioFIFO (int maxCapacity)
{
	setMaximumSize (maxCapacity);
}

template <typename SampleType, size_t numChannels>
void MultiAudioFIFO<SampleType, numChannels>::setMaximumSize (int maximumCapacitySamples)
{
	for (auto& fifo : fifos)
		fifo.setMaximumSize (maximumCapacitySamples);

	capacity = maximumCapacitySamples;
}

template <typename SampleType, size_t numChannels>
int MultiAudioFIFO<SampleType, numChannels>::numStoredSamples() const
{
	int num = std::numeric_limits<int>::max();

	for (auto& fifo : fifos)
		if (auto stored = fifo.numStoredSamples(); stored < num)
			num = stored;

	jassert (num < std::numeric_limits<int>::max());
	return num;
}

template <typename SampleType, size_t numChannels>
void MultiAudioFIFO<SampleType, numChannels>::pushSamples (const AudioBuffer<SampleType>& input)
{
	const auto numSamples = input.getNumSamples();
	const auto channels   = std::min (input.getNumChannels(), static_cast<int>(numChannels));

	for (int i = 0; i < channels; ++i)
		fifos[static_cast<size_t>(i)].pushSamples (input.getReadPointer (i), numSamples);
}

template <typename SampleType, size_t numChannels>
void MultiAudioFIFO<SampleType, numChannels>::popSamples (AudioBuffer<SampleType>& output)
{
	const auto numSamples = output.getNumSamples();
	const auto channels   = std::min (output.getNumChannels(), static_cast<int>(numChannels));

	for (int i = 0; i < channels; ++i)
		fifos[static_cast<size_t>(i)].popSamples (output.getWritePointer (i), numSamples);

	for (int i = channels; i < output.getNumChannels(); ++i)
		output.clear (i, 0, numSamples);
}


template class MultiAudioFIFO<float, 1>;
template class MultiAudioFIFO<double, 1>;
template class MultiAudioFIFO<float, 2>;
template class MultiAudioFIFO<double, 2>;

}  // namespace lemons::dsp
