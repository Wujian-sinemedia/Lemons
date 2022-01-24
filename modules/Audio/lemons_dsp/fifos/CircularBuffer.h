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

#include <juce_audio_basics/juce_audio_basics.h>
#include <lemons_core/lemons_core.h>

namespace lemons::dsp
{

template <typename SampleType>
using AudioBuffer = juce::AudioBuffer<SampleType>;

/** @defgroup lemons_dsp lemons_dsp
	@ingroup Audio
	DSP utility classes.
 */

/** @defgroup lemons_audio_fifos FIFOs
	@ingroup lemons_dsp
	Audio and MIDI FIFOs.
 */

/** @ingroup lemons_audio_fifos
	A thread-safe circular buffer meant for storing a single channel's worth of audio samples.
	If you need to store more than one channel's worth of samples, use the AudioFifo class.
	@see AudioFifo, AudioAndMidiFIFO
 */
template <typename SampleType>
class CircularBuffer final
{
public:
	/** Creates a CircularBuffer with an initial size. */
	explicit CircularBuffer (int initialCapacity = 512);

	/** Stores samples in the buffer.
		If the buffer's capacity isn't big enough to hold all the passed samples, an assertion will be thrown.
	*/
	void storeSamples (const AudioBuffer<SampleType>& samples, int channel = 0);

	/** Stores samples in the buffer.
		If the buffer's capacity isn't big enough to hold all the passed samples, an assertion will be thrown.
	*/
	void storeSamples (const SampleType* samples, int numSamples);

	/** Reads samples from the buffer.
		If you request more samples than are in the buffer, the first section of the output buffer will be filled with zeroes.
	*/
	void getSamples (AudioBuffer<SampleType>& output, int channel = 0);

	/** Reads samples from the buffer.
		If you request more samples than are in the buffer, the first section of the output buffer will be filled with zeroes.
	*/
	void getSamples (SampleType* output, int numSamples);

	/** Returns the total capacity of the buffer. */
	[[nodiscard]] int getCapacity() const noexcept;

	/** Returns the number of samples currently stored in the buffer. */
	[[nodiscard]] int getNumStoredSamples() const noexcept;

	/** Changes the total capacity of the buffer. Calling this method also clears the buffer.
		@see clear()
	*/
	void resize (int newSize);

	/** Clears any previously stored data from the buffer. */
	void clear();

private:
	AudioBuffer<SampleType> buffer;

	juce::AbstractFifo fifo;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CircularBuffer)
};

}  // namespace lemons::dsp
