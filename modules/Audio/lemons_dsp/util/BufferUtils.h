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


#pragma once

namespace lemons::dsp::buffers
{

/** @ingroup lemons_audio_buffers
	Copies all channels from the source buffer to the destination buffer. */
template <typename SampleType>
void copy (const AudioBuffer<SampleType>& source, AudioBuffer<SampleType>& dest);

/** @ingroup lemons_audio_buffers
	Converts all channels from the source buffer to the other numeric type for the destination buffer. */
template <typename Type1, typename Type2>
void convert (const AudioBuffer<Type1>& source, AudioBuffer<Type2>& dest);

/** @ingroup lemons_audio_buffers
	Returns an alias buffer referring to the memory region owned by the original passed buffer.
	This returns a new AudioBuffer object, but does not allocate any memory.
	If the numChannels argument is -1, the alias buffer will have the same number of channels as the buffer being aliased.
 */
template <typename SampleType>
[[nodiscard]] AudioBuffer<SampleType> getAliasBuffer (AudioBuffer<SampleType>& bufferToAlias,
													  int					   startSample,
													  int					   numSamples,
													  int					   numChannels	 = -1,
													  int					   channelOffset = 0);

}  // namespace lemons::dsp::buffers
