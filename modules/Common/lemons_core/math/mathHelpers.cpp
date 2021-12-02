
namespace lemons::math
{
bool probability (int percentOfTheTime) noexcept
{
	return juce::Random::getSystemRandom().nextInt (100) < percentOfTheTime;
}

double sampsToMs (double samplerate, int numSamples) noexcept
{
	jassert (samplerate > 0.0);
	return (static_cast<double> (numSamples) / samplerate) * 1000.;
}

}  // namespace lemons::math


/*---------------------------------------------------------------------------------------------------------------------------------*/


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
            const auto midi = static_cast<float>(rand.nextInt (128));
            const auto freq = math::midiToFreq (midi);
            
            expectEquals (math::freqToMidi (freq),
                          midi);
        }
	}
	{
		const auto subtest = beginSubtest ("Frequency to MIDI");
        
//        for (int i = 0; i < getNumTestingRepetitions(); ++i)
//        {
//            const auto freq = rand.nextInt (6000);
//            const auto midi = math::freqToMidi (freq);
//
//            expectEquals (math::midiToFreq (midi), freq);
//        }
	}

	{
		const auto subtest = beginSubtest ("Hard-coded correct pairs");

		struct MidiFreqPair
		{
			constexpr MidiFreqPair (double midiToUse, double freqToUse)
			    : midi (midiToUse)
			    , freq (freqToUse)
			{
			}

			double midi, freq;
		};

		for (const auto pair : { MidiFreqPair (0., 8.1757989156),
		                         MidiFreqPair (39., 77.7817459305),
		                         MidiFreqPair (57., 220.),
		                         MidiFreqPair (69., 440.) })
		{
			const auto estMidi = math::freqToMidi (pair.freq);
			const auto estFreq = math::midiToFreq (pair.midi);

			constexpr auto midiError = 0.0001;
			constexpr auto freqError = 0.0001;

			expectWithinAbsoluteError (estMidi, pair.midi, midiError);
			expectWithinAbsoluteError (estFreq, pair.freq, freqError);

			expectWithinAbsoluteError (math::freqToMidi (estFreq), pair.midi, midiError);
			expectWithinAbsoluteError (math::midiToFreq (estMidi), pair.freq, freqError);
		}
	}


	for (const auto samplerate : getTestingSamplerates())
	{
		beginTest ("Samplerate: " + String (samplerate));

		{
			const auto subtest = beginSubtest ("Frequency to period");
            
//            for (int i = 0; i < getNumTestingRepetitions(); ++i)
//            {
//                const auto freq      = rand.nextInt (3500);
//                const auto estPeriod = math::periodInSamples (samplerate, freq);
//
//                expectWithinAbsoluteError (math::freqFromPeriod (samplerate, estPeriod), freq, 20);
//            }
		}
		{
			const auto subtest = beginSubtest ("Period to frequency");
            
            for (int i = 0; i < getNumTestingRepetitions(); ++i)
            {
                const auto period  = rand.nextInt (450);
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
