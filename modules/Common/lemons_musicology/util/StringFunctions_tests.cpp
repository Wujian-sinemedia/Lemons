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

#if ! LEMONS_UNIT_TESTS
#	error
#endif

namespace lemons::tests
{

struct StringFunctionsTests final : public CoreTest
{
	StringFunctionsTests()
		: CoreTest ("String functions")
	{
	}

private:

	void runTest() final
	{
		using namespace lemons::music;

		beginTest ("pitch class / string conversion");

		const auto testPitchClass = [&] (int pitchClass, const String& string)
		{
			expectEquals (stringToPitchClass (string), pitchClass);
			expectEquals (pitchClassToString (pitchClass), string);
		};

		testPitchClass (0, "C");
		testPitchClass (2, "D");
		testPitchClass (4, "E");
		testPitchClass (5, "F");
		testPitchClass (7, "G");
		testPitchClass (9, "A");
		testPitchClass (11, "B");

		const auto testPitchClassAccidental = [&] (int pitchClass, const String& sharpString, const String& flatString)
		{
			expectEquals (stringToPitchClass (sharpString), pitchClass);
			expectEquals (stringToPitchClass (flatString), pitchClass);

			expectEquals (pitchClassToString (pitchClass, true), sharpString);
			expectEquals (pitchClassToString (pitchClass, false), flatString);
		};

		testPitchClassAccidental (1, "C#", "Db");
		testPitchClassAccidental (3, "D#", "Eb");
		testPitchClassAccidental (6, "F#", "Gb");
		testPitchClassAccidental (8, "G#", "Ab");
		testPitchClassAccidental (10, "A#", "Bb");


		beginTest ("MIDI pitch / String conversion");

		const auto testPair = [&] (int midiNote, const String& string)
		{
			expectEquals (pitchToString (midiNote), string);
			expectEquals (stringToPitch (string), midiNote);
		};

		testPair (69, "A4");
		testPair (86, "D6");
		testPair (41, "F2");

		const auto testAccidental = [&] (int midiNote, const String& flatString, const String& sharpString)
		{
			expectEquals (pitchToString (midiNote, true), sharpString);
			expectEquals (pitchToString (midiNote, false), flatString);

			expectEquals (stringToPitch (sharpString), midiNote);
			expectEquals (stringToPitch (flatString), midiNote);
		};

		testAccidental (70, "Bb4", "A#4");
		testAccidental (87, "Eb6", "D#6");
		testAccidental (30, "Gb1", "F#1");


		beginTest ("MIDI pan / string conversion");

		const auto testMidiPan = [&] (int pan, const String& string)
		{
			expectEquals (midiPanIntToString (pan), string);
			expectEquals (midiPanStringToInt (string), pan);
		};

		testMidiPan (64, "C");
		testMidiPan (65, "1R");
		testMidiPan (127, "50R");
		testMidiPan (63, "1L");
		testMidiPan (0, "50L");
	}
};

static StringFunctionsTests stringFunctionsTest;

}  // namespace lemons::tests
