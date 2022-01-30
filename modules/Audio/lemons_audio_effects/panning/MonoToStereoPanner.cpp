
/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 *  ======================================================================================
 */

namespace lemons::dsp::FX
{
template <typename SampleType>
void MonoToStereoPanner<SampleType>::prepare (double sr, int blocksize)
{
	left.prepare (sr, blocksize);
	right.prepare (sr, blocksize);
}

template <typename SampleType>
void MonoToStereoPanner<SampleType>::reset()
{
	left.reset();
	right.reset();
}


template <typename SampleType>
void MonoToStereoPanner<SampleType>::process (const AudioBuffer& monoInput,
											  AudioBuffer&		 stereoOutput)
{
	stereoOutput.clear();
	jassert (stereoOutput.getNumChannels() >= 2);
	jassert (monoInput.getNumSamples() == stereoOutput.getNumSamples());

	left.setGain (PannerBase::getLeftGain());
	right.setGain (PannerBase::getRightGain());

	const auto numSamples = stereoOutput.getNumSamples();

	using FVO = juce::FloatVectorOperations;

	FVO::copy (stereoOutput.getWritePointer (0), monoInput.getReadPointer (0), numSamples);
	FVO::copy (stereoOutput.getWritePointer (1), monoInput.getReadPointer (0), numSamples);

	auto leftAlias	= buffers::getAliasBuffer (stereoOutput, 0, numSamples, 1);
	auto rightAlias = buffers::getAliasBuffer (stereoOutput, 0, numSamples, 1, 1);

	left.process (leftAlias);
	right.process (rightAlias);
}

template class MonoToStereoPanner<float>;
template class MonoToStereoPanner<double>;

}  // namespace lemons::dsp::FX
