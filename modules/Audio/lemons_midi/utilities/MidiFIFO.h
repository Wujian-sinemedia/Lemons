
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

#pragma once

namespace lemons::midi
{

/** @ingroup lemons_midi
	A FIFO for storing MIDI events.
	@see AudioAndMidiFIFO
 */
class MidiFIFO final
{
public:

	/** Creates a MIDI FIFO with an initial capacity, in messages. */
	explicit MidiFIFO (int maxNumMessages = 512);

	/** Destructor. */
	~MidiFIFO() = default;

	/** Pushes MIDI events into the FIFO. */
	void pushEvents (const MidiBuffer& source, int numSamples);

	/** Pops MIDI events from the FIFO. */
	void popEvents (MidiBuffer& output, int numSamples);

	/** Returns the number of MIDI messages currently stored in the FIFO.
		@see numStoredSamples()
	*/
	[[nodiscard]] int numStoredEvents() const noexcept;

	/** Returns the number of samples that the FIFO's stored messages represent. Note that this may be different than the actual number of messages stored.
		@see numStoredEvents()
	*/
	[[nodiscard]] int numStoredSamples() const noexcept;

	/** Changes the capacity of the FIFO. Calling this method clears any previously stored data from the FIFO. */
	void setSize (int maxNumMessages);

	/** Clears all data from the FIFO. */
	void clear();

private:

	int numSamplesStored { 0 };

	MidiBuffer base, copying;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiFIFO)
};

}  // namespace lemons::midi
