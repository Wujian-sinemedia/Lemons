
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

#pragma once

#include <lemons_midi/lemons_midi.h>

namespace lemons::dsp
{

using juce::MidiBuffer;

/** @ingroup lemons_audio_fifos
	A FIFO for storing both audio and MIDI data together. Note that this class is not thread-safe.
	@see AudioFifo, MidiFIFO
 */
template <typename SampleType>
class AudioAndMidiFIFO final
{
public:

	/** Creates a FIFO with an initial capacity. */
	explicit AudioAndMidiFIFO (int samples = 1024);

	/** Pushes audio and MIDI into the FIFO. */
	void push (const AudioBuffer<SampleType>& audioIn, const MidiBuffer& midiIn);

	/** Pops audio and MIDI from the FIFO. */
	void pop (AudioBuffer<SampleType>& audioOut, MidiBuffer& midiOut);

	/** Returns the number of samples' worth of data currently stored in the FIFO. */
	[[nodiscard]] int numStoredSamples() const noexcept;

	/** Resizes the FIFO's internal capacity. Calling this method also clears any data stored in the FIFO. */
	void setSize (int numSamples, int numChannels = 2);

	/** Clears the FIFO. */
	void clear();

private:

	midi::MidiFIFO		  midi;
	AudioFifo<SampleType> audio;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioAndMidiFIFO)
};

}  // namespace lemons::dsp
