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

#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

MathTests::MathTests()
    : CoreTest ("Math tests")
{
}

void MathTests::runTest()
{
	beginTest ("numberIsEven");

	for (const auto even : { 2, 4, 6, 12, 24, 138 })
		expect (math::numberIsEven (even));

	for (const auto odd : { 1, 3, 5, 15, 23, 241 })
		expect (! math::numberIsEven (odd));


	beginTest ("MIDI/Frequency conversion");

	auto rand = getRandom();

	{
		const auto subtest = beginSubtest ("MIDI to frequency");

		for (int i = 0; i < getNumTestingRepetitions(); ++i)
		{
			const auto midi = static_cast<float> (rand.nextInt (128));
			const auto freq = math::midiToFreq (midi);

			expectWithinAbsoluteError (math::freqToMidi (freq), midi, 0.001f);
		}
	}
	{
		//		const auto subtest = beginSubtest ("Frequency to MIDI");
		//
		//        for (int i = 0; i < getNumTestingRepetitions(); ++i)
		//        {
		//            const auto freq = rand.nextInt (6000);
		//            const auto midi = math::freqToMidi (freq);
		//
		//            expectWithinAbsoluteError (math::midiToFreq (midi), freq, 5);
		//        }
	}

	{
		const auto subtest = beginSubtest ("Hard-coded correct pairs");

		const auto testPair = [&] (double midi, double freq)
		{
			const auto estMidi = math::freqToMidi (freq);
			const auto estFreq = math::midiToFreq (midi);

			constexpr auto midiError = 0.0001;
			constexpr auto freqError = 0.0001;

			expectWithinAbsoluteError (estMidi, midi, midiError);
			expectWithinAbsoluteError (estFreq, freq, freqError);

			expectWithinAbsoluteError (math::freqToMidi (estFreq), midi, midiError);
			expectWithinAbsoluteError (math::midiToFreq (estMidi), freq, freqError);
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
			//			const auto subtest = beginSubtest ("Frequency to period");
			//
			//            for (int i = 0; i < getNumTestingRepetitions(); ++i)
			//            {
			//                const auto freq      = rand.nextInt (3500);
			//                const auto estPeriod = math::periodInSamples (samplerate, freq);
			//
			//                expectWithinAbsoluteError (math::freqFromPeriod (samplerate, estPeriod), freq, 20);
			//            }
		} {
			const auto subtest = beginSubtest ("Period to frequency");

			for (int i = 0; i < getNumTestingRepetitions(); ++i)
			{
				const auto period = rand.nextInt ({ 1, 450 });

				const auto estFreq = math::freqFromPeriod (samplerate, period);

				expectWithinAbsoluteError (math::periodInSamples (samplerate, estFreq), period, 2);
			}
		}

		{
			const auto subtest = beginSubtest ("Samples to milliseconds");

			for (int i = 0; i < getNumTestingRepetitions(); ++i)
			{
				const auto samples = rand.nextInt (1024);
				const auto estMs   = math::sampsToMs (samplerate, samples);

				expectEquals (math::msToSamps (samplerate, estMs), samples);
			}
		}
		{
			const auto subteset = beginSubtest ("Milliseconds to samples");

			for (int i = 0; i < getNumTestingRepetitions(); ++i)
			{
				const auto ms         = rand.nextDouble();
				const auto estSamples = math::msToSamps (samplerate, ms);

				expectWithinAbsoluteError (math::sampsToMs (samplerate, estSamples), ms, 0.015);
			}
		}
	}
}

}  // namespace lemons::tests

#endif
