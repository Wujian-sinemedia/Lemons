#pragma once

namespace lemons::midi
{

/** A FIFO for storing MIDI events.
    @see dsp::AudioAndMidiFIFO
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


/*---------------------------------------------------------------------------------------------------------------------------------*/


#if LEMONS_UNIT_TESTS

#  include <lemons_unit_testing/lemons_unit_testing.h>

namespace lemons::tests
{

struct MidiFifoTests : public DspTest
{
public:
	MidiFifoTests();

private:
	void runTest() final;

	midi::MidiFIFO fifo;

	juce::MidiBuffer input, output;
};

static MidiFifoTests midiFifoTest;

}  // namespace lemons::tests

#endif
