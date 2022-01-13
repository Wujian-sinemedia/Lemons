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

template <typename FloatType>
class PitchDetectorTests final : public AllOscillatorsTest<FloatType>
{
public:

	explicit PitchDetectorTests()
	    : AllOscillatorsTest<FloatType> ("Pitch detector tests")
	{
		detector.setConfidenceThresh (0.15);
	}

private:
	void runOscillatorTest (dsp::Oscillator<FloatType>& osc) final
	{
		for (const auto samplerate : this->getTestingSamplerates())
		{
			const auto samplerateSubtest = this->beginSubtest ("Samplerate: " + String (samplerate));

			const auto latency = detector.setSamplerate (samplerate);

			storage.setSize (1, latency, true, true, true);

			const auto testFreq = [&] (const float correctFreq)
			{
				osc.setFrequency (static_cast<FloatType> (correctFreq),
				                  static_cast<FloatType> (samplerate));

				osc.getSamples (storage);

				const auto estFreq = detector.detectPitch (storage);

				this->expectWithinAbsoluteError (estFreq, correctFreq, 20.f);

				this->expectWithinAbsoluteError (math::freqToMidi (estFreq),
				                                 math::freqToMidi (correctFreq),
				                                 0.2f);
			};

			testFreq (440.f);
			testFreq (minDetectableFreq);
			testFreq (maxDetectableFreq);

			auto rand = this->getRandom();

			for (int i = 0; i < this->getNumTestingRepetitions(); ++i)
				testFreq (juce::jmap (rand.nextFloat(), minDetectableFreq, maxDetectableFreq));
		}
	}

	void runOtherTests() final
	{
		{
			const auto subtest = this->beginSubtest ("Detect random noise as unpitched");

			for (int r = 0; r < this->getNumTestingRepetitions(); ++r)
			{
				fillAudioBufferWithRandomNoise (storage, this->getRandom());

				this->expectEquals (detector.detectPitch (storage), 0.f);
			}
		}

//		const auto subtest = this->beginSubtest ("Detect silence as unpitched");
//
//		storage.clear();
//
//		for (int r = 0; r < this->getNumTestingRepetitions(); ++r)
//			this->expectEquals (detector.detectPitch (storage), 0.f);
	}

	static constexpr auto minDetectableFreq = 30.f;
	static constexpr auto maxDetectableFreq = 3000.f;

	AudioBuffer<FloatType> storage;

	dsp::psola::PitchDetector<FloatType> detector { juce::roundToInt (minDetectableFreq) };
};

template struct PitchDetectorTests<float>;
template struct PitchDetectorTests<double>;

LEMONS_CREATE_DSP_TEST (PitchDetectorTests)

}  // namespace lemons::tests
