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

#if ! LEMONS_UNIT_TESTS
#  error
#endif

namespace lemons::tests
{

using namespace lemons::math;

struct MathTests final : public CoreTest
{
	explicit MathTests()
		: CoreTest ("Math tests")
	{
	}

private:

	void runTest() final
	{
		beginTest ("MIDI/Frequency conversion");

		auto rand = getRandom();

		{
			const auto subtest = beginSubtest ("MIDI to frequency");

			for (int i = 0; i < getNumTestingRepetitions(); ++i)
			{
				const auto midi = static_cast<double> (rand.nextInt (128));
				const auto freq = midiToFreq (midi);

				expectWithinAbsoluteError (freqToMidi (freq), midi, 0.001);
			}
		}
		{
			const auto subtest = beginSubtest ("Frequency to MIDI");

			for (int i = 0; i < getNumTestingRepetitions(); ++i)
			{
				const auto freq = static_cast<double> (rand.nextInt ({ 80, 4500 }));
				const auto midi = freqToMidi (freq);

				expectWithinAbsoluteError (midiToFreq (midi), freq, 0.001);
			}
		}

		{
			const auto subtest = beginSubtest ("Hard-coded correct pairs");

			const auto testPair = [&] (double midi, double freq)
			{
				const auto estMidi = freqToMidi (freq);
				const auto estFreq = midiToFreq (midi);

				expectWithinAbsoluteError (estMidi, midi, 0.0001);
				expectWithinAbsoluteError (estFreq, freq, 0.0001);

				expectWithinAbsoluteError (freqToMidi (estFreq), midi, 0.0001);
				expectWithinAbsoluteError (midiToFreq (estMidi), freq, 0.0001);
			};

			testPair (0., 8.1757989156);
			testPair (39., 77.7817459305);
			testPair (57., 220.);
			testPair (69., 440.);
		}

		for (const auto samplerate : getTestingSamplerates())
		{
			beginTest ("Samplerate: " + String (samplerate));

			{
				const auto subtest = beginSubtest ("Period to frequency");

				for (int i = 0; i < getNumTestingRepetitions(); ++i)
				{
					const auto period  = static_cast<double> (rand.nextInt ({ 1, 450 }));
					const auto estFreq = freqFromPeriod (samplerate, period);

					expectWithinAbsoluteError (static_cast<double> (periodInSamples (samplerate, estFreq)), period, 1.);
				}
			}

			{
				const auto subtest = beginSubtest ("Samples to milliseconds");

				for (int i = 0; i < getNumTestingRepetitions(); ++i)
				{
					const auto samples = rand.nextInt (1024);
					const auto estMs   = sampsToMs (samplerate, samples);

					expectEquals (msToSamps (samplerate, estMs), samples);
				}
			}
			{
				const auto subteset = beginSubtest ("Milliseconds to samples");

				for (int i = 0; i < getNumTestingRepetitions(); ++i)
				{
					const auto ms		  = rand.nextDouble();
					const auto estSamples = msToSamps (samplerate, ms);

					expectWithinAbsoluteError (sampsToMs (samplerate, estSamples), ms, 0.015);
				}
			}
		}
	}
};

static MathTests mathFunctionsTest;


static_assert (isDivisibleBy (9, 3), "isDivisibleBy test");
static_assert (isDivisibleBy (15, 5), "isDivisibleBy test");
static_assert (isDivisibleBy (32, 8), "isDivisibleBy test");
static_assert (! isDivisibleBy (8, 7), "isDivisibleBy test");
static_assert (! isDivisibleBy (9, 2), "isDivisibleBy test");

static_assert (numberIsEven (16), "numberIsEven test");
static_assert (numberIsEven (126), "numberIsEven test");
static_assert (! numberIsEven (3), "numberIsEven test");
static_assert (! numberIsEven (521), "numberIsEven test");


}  // namespace lemons::tests
