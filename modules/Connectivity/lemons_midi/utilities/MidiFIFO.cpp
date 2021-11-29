
namespace lemons::midi
{

MidiFIFO::MidiFIFO (int maxNumMessages) { setSize (maxNumMessages); }

void MidiFIFO::setSize (int maxNumMessages)
{
	jassert (maxNumMessages > 0);

	clear();

	const auto messages = static_cast<size_t> (maxNumMessages);
	base.ensureSize (messages);
	copying.ensureSize (messages);
}

void MidiFIFO::clear()
{
	base.clear();
	copying.clear();
	numSamplesStored = 0;
}

int MidiFIFO::numStoredEvents() const noexcept { return base.getNumEvents(); }

int MidiFIFO::numStoredSamples() const noexcept { return numSamplesStored; }

void MidiFIFO::pushEvents (const MidiBuffer& source, int numSamples)
{
	base.addEvents (source, 0, numSamples, numSamplesStored);
	numSamplesStored += numSamples;
}

void MidiFIFO::popEvents (MidiBuffer& output, int numSamples)
{
	output.clear();
	output.addEvents (base, 0, numSamples, 0);

	// Move all the remaining events forward by the number of samples removed
	copying.clear();
	copying.addEvents (base, numSamples, numSamplesStored, -numSamples);

	base.swapWith (copying);
	numSamplesStored = std::max (0, numSamplesStored - numSamples);
}

}  // namespace lemons::midi


/*---------------------------------------------------------------------------------------------------------------------------------*/


#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

MidiFifoTests::MidiFifoTests()
    : DspTest ("MidiFifoTests")
{
}

void MidiFifoTests::runTest()
{
	auto rand = getRandom();

	for (const auto numSamples : getTestingBlockSizes())
	{
		logBlocksizeMessage (numSamples);

		const auto numEvents = numSamples / 2;

		fifo.clear();
		input.clear();

		for (int i = 0; i < numEvents; ++i)
			input.addEvent (juce::MidiMessage::controllerEvent (1, rand.nextInt (128), rand.nextInt (128)),
			                i);


		beginTest ("Num events stored correctly");

		fifo.pushEvents (input, numSamples);

		expectEquals (fifo.numStoredEvents(), numEvents);


		beginTest ("Num stored samples stored correctly");

		expectEquals (fifo.numStoredSamples(), numSamples);


		beginTest ("Store events and retrieve later");

		fifo.popEvents (output, numSamples);

		expect (midiBuffersAreEqual (input, output));


		beginTest ("Resizing clears the FIFO");

		fifo.setSize (numSamples / 4);

		expectEquals (fifo.numStoredEvents(), 0);
		expectEquals (fifo.numStoredSamples(), 0);
	}
}

}  // namespace lemons::tests

#endif
