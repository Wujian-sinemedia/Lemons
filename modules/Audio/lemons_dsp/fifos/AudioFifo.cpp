namespace lemons::dsp
{

template <typename SampleType>
void AudioFifo<SampleType>::pushSamples (const AudioBuffer<SampleType>& input)
{
	const auto numSamples = input.getNumSamples();

	for (int chan = 0;
	     chan < std::min (buffers.size(), input.getNumChannels());
	     ++chan)
	{
		buffers.getUnchecked (chan)->storeSamples (input.getReadPointer (chan),
		                                           numSamples);
	}
}

template <typename SampleType>
void AudioFifo<SampleType>::popSamples (AudioBuffer<SampleType>& output)
{
	const auto numSamples = output.getNumSamples();

	for (int chan = 0;
	     chan < std::min (buffers.size(), output.getNumChannels());
	     ++chan)
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

	return buffers.getUnchecked (0)->getNumStoredSamples();
}

template <typename SampleType>
void AudioFifo<SampleType>::resize (int maxNumSamples, int numChannels)
{
	if (buffers.size() < numChannels)
	{
        while (buffers.size() < numChannels)
            buffers.add (new CircularBuffer<SampleType>());
	}
	else
	{
        while (buffers.size() > numChannels)
            buffers.removeLast();
	}

	for (auto* buffer : buffers)
		buffer->resize (maxNumSamples);
}

template class AudioFifo<float>;
template class AudioFifo<double>;

}  // namespace lemons::dsp
