#if ! LEMONS_UNIT_TESTS
#  error
#endif

namespace lemons::tests
{

template <typename FloatType>
struct PitchDetectorTests : public DspTest
{
public:
	explicit PitchDetectorTests()
	    : DspTest (getDspTestName<FloatType> ("Pitch detector tests"))
	{
	}

private:
	void runTest() final
	{
		for (const auto confidenceThresh : getConfidenceThresholdsToTest())
		{
			beginTest ("YIN confidence threshold: " + String (confidenceThresh));

			detector.setConfidenceThresh (confidenceThresh);

			for (const auto samplerate : getTestingSamplerates())
			{
				const auto samplerateSubtest = beginSubtest ("Samplerate: " + String (samplerate));

				const auto latency = detector.setSamplerate (samplerate);

				storage.setSize (1, latency, true, true, true);

				{
					const auto subtest = beginSubtest ("Detect frequencies of oscillators");
					runOscillatorTest (sine, samplerate, "Sine");
					runOscillatorTest (saw, samplerate, "Saw");
					runOscillatorTest (square, samplerate, "Square");
					runOscillatorTest (triangle, samplerate, "Triangle");
				}

				//                for (const auto detune : { 0, 1, 5, 12 })
				//                {
				//                    logImportantMessage (String("Setting supersaw pitch spread to ") + String(detune));
				//
				//                    superSaw.setDetuneAmount (detune);
				//                    runOscillatorTest (superSaw, "SuperSaw", samplerate, 1);
				//                }

				{
					const auto subtest = beginSubtest ("Detect random noise as unpitched");

					for (int r = 0; r < getNumTestingRepetitions(); ++r)
					{
						fillAudioBufferWithRandomNoise (storage, getRandom());

						expectEquals (detector.detectPitch (storage), 0.f);
					}
				}

				const auto subtest = beginSubtest ("Detect silence as unpitched");

				storage.clear();

				for (int r = 0; r < getNumTestingRepetitions(); ++r)
					expectEquals (detector.detectPitch (storage), 0.f);
			}
		}
	}

	void runOscillatorTest (dsp::osc::Oscillator<FloatType>& osc, double samplerate, const String& waveName)
	{
		const auto subtest = beginSubtest (waveName + " wave");

		const auto testFreq = [&] (const float correctFreq)
		{
			osc.setFrequency (static_cast<FloatType> (correctFreq),
			                  static_cast<FloatType> (samplerate));

			osc.getSamples (storage);

			const auto estFreq = detector.detectPitch (storage);

			expectWithinAbsoluteError (estFreq, correctFreq, 20.f);

			expectWithinAbsoluteError (math::freqToMidi (estFreq),
			                           math::freqToMidi (correctFreq),
			                           0.2f);
		};

		testFreq (440.f);
		testFreq (minDetectableFreq);
		testFreq (maxDetectableFreq);

		auto rand = getRandom();

		for (int i = 0; i < getNumTestingRepetitions(); ++i)
			testFreq (juce::jmap (rand.nextFloat(), minDetectableFreq, maxDetectableFreq));
	}

	static const std::vector<float> getConfidenceThresholdsToTest()
	{
		if (testingIntensityIsLow())
			return { 0.15f };

		return { 0.1f, 0.15f, 0.2f };
	}

	static constexpr auto minDetectableFreq = 30.f;
	static constexpr auto maxDetectableFreq = 3000.f;

	AudioBuffer<FloatType> storage;

	dsp::psola::PitchDetector<FloatType> detector { juce::roundToInt (minDetectableFreq) };

	dsp::osc::Sine<FloatType>     sine;
	dsp::osc::Saw<FloatType>      saw;
	dsp::osc::Square<FloatType>   square;
	dsp::osc::Triangle<FloatType> triangle;
	dsp::osc::SuperSaw<FloatType> superSaw;
};

template struct PitchDetectorTests<float>;
template struct PitchDetectorTests<double>;

LEMONS_CREATE_DSP_TEST (PitchDetectorTests)

}  // namespace lemons::tests
