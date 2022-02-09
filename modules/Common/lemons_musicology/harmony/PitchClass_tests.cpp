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

struct PitchClassTests final : public CoreTest
{
	PitchClassTests()
		: CoreTest ("Pitch class")
	{
	}

private:

	void runTest() final
	{
		using namespace lemons::music;

		beginTest ("pitch class / string conversion");

		auto testPitchClass = [this] (int pitchClass, const String& string)
		{
			const PitchClass fromString { string };
			const PitchClass fromInt { pitchClass };

			expectEquals (fromString.getAsInt(), fromInt.getAsInt());
			expectEquals (fromString.getAsInt(), pitchClass);
			expectEquals (fromInt.getAsString(), string);
		};

		testPitchClass (0, "C");
		testPitchClass (2, "D");
		testPitchClass (4, "E");
		testPitchClass (5, "F");
		testPitchClass (7, "G");
		testPitchClass (9, "A");
		testPitchClass (11, "B");

		auto testPitchClassAccidental = [this] (int pitchClass, const String& sharpString, const String& flatString)
		{
			const PitchClass fromSharpString { sharpString };
			const PitchClass fromFlatString { flatString };

			expectEquals (fromSharpString.getAsInt(), pitchClass);
			expectEquals (fromFlatString.getAsInt(), pitchClass);
		};

		testPitchClassAccidental (1, "C#", "Db");
		testPitchClassAccidental (3, "D#", "Eb");
		testPitchClassAccidental (6, "F#", "Gb");
		testPitchClassAccidental (8, "G#", "Ab");
		testPitchClassAccidental (10, "A#", "Bb");
	}
};

static PitchClassTests pitchClassTests;

}  // namespace lemons::tests
