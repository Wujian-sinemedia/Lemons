#if ! LEMONS_UNIT_TESTS
#  error
#endif

namespace lemons::tests
{

struct MidiSerializingTests final : public CoreTest
{
	explicit MidiSerializingTests()
		: CoreTest ("MIDI serializing")
	{
	}

private:

	void runTest() final
	{
		beginTest ("MIDI serializing");

		MidiBuffer origMidi;

		fillMidiBufferWithRandomEvents (origMidi, 256, getRandom());

		{
			const auto subtest = beginSubtest ("MIDI buffer to/from MIDI file");

			const auto file	   = midi::midiBufferToFile (origMidi);
			const auto decoded = midi::midiBufferFromFile (file);

			expect (midiBuffersAreEqual (origMidi, decoded));
		}

		{
			const auto block   = serializing::midiToBinary (origMidi);
			const auto decoded = serializing::midiBufferFromBinary (block);

			//        expect (midiBuffersAreEqual (origMidi, decoded));
		}

		{
			const auto subtest = beginSubtest ("MidiBuffer");

			MidiBuffer orig;

			fillMidiBufferWithRandomEvents (orig, getRandom().nextInt ({ 1, 100 }), getRandom());

			//        expect (midiBuffersAreEqual (orig, toVarAndBack (orig)));
		}

		//    {
		//        const auto subtest = beginSubtest ("MidiFile");
		//    }

		{
			const auto subtest = beginSubtest ("MidiMessage");

			constexpr auto channel	= 1;
			constexpr auto note		= 78;
			constexpr auto velocity = 0.38f;

			const auto orig = juce::MidiMessage::noteOn (channel, note, velocity);

			const auto decoded = toVarAndBack (orig);

			expect (decoded.isNoteOn());

			expectEquals (decoded.getChannel(), channel);
			expectEquals (decoded.getNoteNumber(), note);

			expectWithinAbsoluteError (decoded.getFloatVelocity(), velocity, 0.01f);
		}
	}
};

static MidiSerializingTests midiSerializingTest;

}  // namespace lemons::tests
