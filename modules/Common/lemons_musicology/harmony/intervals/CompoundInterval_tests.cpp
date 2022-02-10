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

struct CompoundIntervalTests : public CoreTest
{
	CompoundIntervalTests()
		: CoreTest ("Compound intervals")
	{
	}

private:

	void runTest() final
	{
		/*
		 - create from num semitones
		 - create from pitches
		 - ==, isEnharmonicTo
		 - >, <
		 - get num semitones
		 - hasSameSimpleInterval
		 */

		using namespace lemons::music;

		using Q = Interval::Quality;

		beginTest ("Compound interval tests");

		{
			const auto subtest = beginSubtest ("Constructors");

			auto test_constructors = [this] (Q quality, int simpleKind, int octaves)
			{
				const Interval simple { simpleKind, quality };

				const auto compoundKind = (octaves * 7) + simpleKind;

				const CompoundInterval interval1 { compoundKind, quality };
				const CompoundInterval interval2 { octaves, simple };

				expect (interval1 == interval2);

				expectEquals (interval1.getNumOctaves(), octaves);
				expectEquals (interval2.getNumOctaves(), octaves);

				expect (interval1.getSimpleInterval() == simple);
				expect (interval2.getSimpleInterval() == simple);

				expectEquals (interval1.getKind(), compoundKind);
				expectEquals (interval2.getKind(), compoundKind);
			};

			for (auto octaves = 0; octaves < 5; ++octaves)
			{
				test_constructors (Q::Minor, 2, octaves);
				test_constructors (Q::Major, 2, octaves);
				test_constructors (Q::Diminished, 3, octaves);
				test_constructors (Q::Minor, 3, octaves);
				test_constructors (Q::Major, 3, octaves);
				test_constructors (Q::Augmented, 3, octaves);
				test_constructors (Q::Diminished, 4, octaves);
				test_constructors (Q::Perfect, 4, octaves);
				test_constructors (Q::Augmented, 4, octaves);
				test_constructors (Q::Diminished, 5, octaves);
				test_constructors (Q::Perfect, 5, octaves);
				test_constructors (Q::Augmented, 5, octaves);
				test_constructors (Q::Diminished, 6, octaves);
				test_constructors (Q::Minor, 6, octaves);
				test_constructors (Q::Major, 6, octaves);
				test_constructors (Q::Augmented, 6, octaves);
				test_constructors (Q::Diminished, 7, octaves);
				test_constructors (Q::Minor, 7, octaves);
				test_constructors (Q::Major, 7, octaves);
				test_constructors (Q::Augmented, 7, octaves);
			}
		}

		{
			const auto subtest = beginSubtest ("Constructor special cases");

			const CompoundInterval interval1 {
				1, Interval {8, Q::Perfect}
			};

			expectEquals (interval1.getNumOctaves(), 2);

			expect (interval1.getSimpleInterval() == Interval());

			const CompoundInterval interval2 {
				1, Interval {8, Q::Augmented}
			};

			expectEquals (interval2.getNumOctaves(), 2);

			expect (interval2.getSimpleInterval() == Interval { 0, Q::Augmented });

			const CompoundInterval interval3 {
				1, Interval {8, Q::Diminished}
			};

			expectEquals (interval3.getNumOctaves(), 1);

			expect (interval3.getSimpleInterval() == Interval { 8, Q::Diminished });
		}
	}
};

static CompoundIntervalTests compoundIntervalTest;

}  // namespace lemons::tests
