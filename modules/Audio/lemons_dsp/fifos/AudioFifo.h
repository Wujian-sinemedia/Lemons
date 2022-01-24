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

#include <lemons_core/lemons_core.h>

namespace lemons::dsp
{

/** @ingroup lemons_audio_fifos
	A multichannel audio FIFO.
	If you only need to store a single channel's worth of audio samples, consider using the CircularBuffer class instead.
	Also note that while the CircularBuffer class is thread-safe, this class is not.
	@see CircularBuffer, AudioAndMidiFIFO
 */
template <typename SampleType>
class AudioFifo final
{
public:

	/** Creates an AudioFifo with an initial capacity. */
	explicit AudioFifo (int numSamples = 512, int numChannels = 2);

	/** Pushes samples into the FIFO.
		If the internal buffer's capacity isn't big enough to hold all the passed samples, an assertion will be thrown.
	*/
	void pushSamples (const AudioBuffer<SampleType>& input);

	/** Retrieves samples from the FIFO.
		If you request more samples than are in the buffer, the first section of the output buffer will be filled with zeroes.
	*/
	void popSamples (AudioBuffer<SampleType>& output);

	/** Returns the number of samples currently stored in the FIFO.
		This searches through every channel and returns the minimum number of samples available for any channel.
	*/
	[[nodiscard]] int numStoredSamples() const noexcept;

	/** Returns the number of channels of audio this FIFO can store. */
	[[nodiscard]] int numChannels() const noexcept;

	/** Clears the FIFO. */
	void clear();

	/** Changes the total capacity of the FIFO. Calling this method also clears the FIFO. */
	void resize (int maxNumSamples, int numChannels = 2);

private:

	ConstructedArray<CircularBuffer<SampleType>> buffers;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioFifo)
};

}  // namespace lemons::dsp
