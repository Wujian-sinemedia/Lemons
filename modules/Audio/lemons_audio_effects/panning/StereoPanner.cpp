
namespace lemons::dsp::FX
{
template <typename SampleType>
void StereoPanner<SampleType>::prepare (double samplerate, int blocksize)
{
	left.prepare (samplerate, blocksize);
	right.prepare (samplerate, blocksize);
}

template <typename SampleType>
void StereoPanner<SampleType>::reset()
{
	left.reset();
	right.reset();
}


template <typename SampleType>
void StereoPanner<SampleType>::process (const AudioBuffer& stereoInput,
                                        AudioBuffer&       stereoOutput)
{
	stereoOutput.clear();
	jassert (stereoInput.getNumChannels() >= 2
	         && stereoOutput.getNumChannels() >= 2);
	jassert (stereoInput.getNumSamples() == stereoOutput.getNumSamples());

	left.setGain (PannerBase::getLeftGain());
	right.setGain (PannerBase::getRightGain());

	const auto numSamples = stereoInput.getNumSamples();

	for (int i = 0; i < 2; ++i)
		juce::FloatVectorOperations::copy (stereoOutput.getWritePointer (i), stereoInput.getReadPointer (i), numSamples);

	auto leftAlias  = buffers::getAliasBuffer (stereoOutput, 0, numSamples, 1);
	auto rightAlias = buffers::getAliasBuffer (stereoOutput, 0, numSamples, 1, 1);

	left.process (leftAlias);
	right.process (rightAlias);
}

template <typename SampleType>
void StereoPanner<SampleType>::process (AudioBuffer& audio)
{
	process (audio, audio);
}

template class StereoPanner<float>;
template class StereoPanner<double>;


}  // namespace lemons::dsp::FX
