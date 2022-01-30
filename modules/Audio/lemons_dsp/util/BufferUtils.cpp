
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

#include <lemons_core/lemons_core.h>

namespace lemons::dsp::buffers
{
template <typename SampleType>
void copy (const AudioBuffer<SampleType>& source, AudioBuffer<SampleType>& dest)
{
	dest.clear();

	const auto numSamples = source.getNumSamples();
	jassert (dest.getNumSamples() >= numSamples);

	const auto numChannels = std::min (source.getNumChannels(), dest.getNumChannels());

	for (auto chan = 0; chan < numChannels; ++chan)
		juce::FloatVectorOperations::copy (dest.getWritePointer (chan),
										   source.getReadPointer (chan),
										   numSamples);
}

template void copy (const AudioBuffer<float>&, AudioBuffer<float>&);
template void copy (const AudioBuffer<double>&, AudioBuffer<double>&);

template <typename Type1, typename Type2>
void convert (const AudioBuffer<Type1>& source, AudioBuffer<Type2>& dest)
{
	static_assert (! std::is_same_v<Type1, Type2>,
				   "Converting between two buffers with the same sample type!");

	const auto numSamples = source.getNumSamples();
	jassert (dest.getNumSamples() >= numSamples);

	dest.makeCopyOf (source, true);
}
template void convert (const AudioBuffer<float>&, AudioBuffer<double>&);
template void convert (const AudioBuffer<double>&, AudioBuffer<float>&);


template <typename SampleType>
AudioBuffer<SampleType> getAliasBuffer (AudioBuffer<SampleType>& bufferToAlias,
										int						 startSample,
										int						 numSamples,
										int						 numChannels,
										int						 channelOffset)
{
	if (numChannels == -1)
		numChannels = bufferToAlias.getNumChannels();

	jassert (numChannels > 0);
	jassert (numSamples >= 0);

	jassert (numChannels <= bufferToAlias.getNumChannels());
	jassert (numSamples <= bufferToAlias.getNumSamples());

	return { bufferToAlias.getArrayOfWritePointers() + channelOffset, numChannels, startSample, numSamples };
}
template AudioBuffer<float>	 getAliasBuffer (AudioBuffer<float>&, int, int, int, int);
template AudioBuffer<double> getAliasBuffer (AudioBuffer<double>&, int, int, int, int);

}  // namespace lemons::dsp::buffers
