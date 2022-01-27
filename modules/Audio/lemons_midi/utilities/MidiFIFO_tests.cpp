#if ! LEMONS_UNIT_TESTS
#	error
#endif

namespace lemons::tests
{

struct MidiFifoTests final : public MidiTest
{
public:

	explicit MidiFifoTests()
		: MidiTest ("MIDI FIFO tests")
	{
	}

private:

	void runTest() final
	{
		for (const auto numSamples : getTestingBlockSizes())
		{
			beginTest ("Blocksize: " + String (numSamples));

			const auto numEvents = numSamples / 2;

			fillMidiBufferWithRandomEvents (input, numEvents, getRandom());

			fifo.clear();

			logImportantMessage ("Num events stored correctly");

			fifo.pushEvents (input, numSamples);

			expectEquals (fifo.numStoredEvents(), numEvents);


			logImportantMessage ("Num stored samples stored correctly");

			expectEquals (fifo.numStoredSamples(), numSamples);


			fifo.popEvents (output, numSamples);

			expect (midiBuffersAreEqual (input, output));


			logImportantMessage ("Resizing clears the FIFO");

			fifo.setSize (numSamples / 4);

			expectEquals (fifo.numStoredEvents(), 0);
			expectEquals (fifo.numStoredSamples(), 0);
		}
	}

	midi::MidiFIFO fifo;

	juce::MidiBuffer input, output;
};

static MidiFifoTests midiFifoTest;

}  // namespace lemons::tests
