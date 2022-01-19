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

	for (auto chan = 0; chan < input.getNumChannels(); ++chan)
	{
		buffers[chan]->storeSamples (input.getReadPointer (chan), numSamples);
	}
}

template <typename SampleType>
void AudioFifo<SampleType>::popSamples (AudioBuffer<SampleType>& output)
{
	jassert (buffers->size() >= output.getNumChannels());

	const auto numSamples = output.getNumSamples();

	for (auto chan = 0; chan < output.getNumChannels(); ++chan)
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
