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

			auto testFreq = [this, &osc, samplerate] (const float correctFreq, bool reset)
			{
				if (reset)
					detector.reset();

				osc.setFrequency (static_cast<FloatType> (correctFreq),
					static_cast<FloatType> (samplerate));

				osc.getSamples (storage);

				const auto estFreq = detector.detectPitch (storage);

				this->expectWithinAbsoluteError (estFreq, correctFreq, 20.f);

				this->expectWithinAbsoluteError (math::freqToMidi (estFreq),
					math::freqToMidi (correctFreq),
					0.2f);
			};

			testFreq (440.f, true);
			testFreq (minDetectableFreq, true);
			testFreq (maxDetectableFreq, true);

			auto rand = this->getRandom();

			for (auto i = 0; i < this->getNumTestingRepetitions(); ++i)
				testFreq (juce::jmap (rand.nextFloat(), minDetectableFreq, maxDetectableFreq), true);

			detector.reset();

			for (auto i = 0; i < this->getNumTestingRepetitions(); ++i)
			{
				testFreq (440.f + i, false);
				testFreq (440.f - i, false);
			}
		}
	}

	void runOtherTests() final
	{
		{
			const auto subtest = this->beginSubtest ("Randomly generated waveform");

			constexpr auto samplerate = 44100.;

			const auto latency = detector.setSamplerate (samplerate);

			storage.setSize (1, latency, true, true, true);

			const auto minPeriod = juce::roundToInt (math::periodInSamples (samplerate, maxDetectableFreq));

			jassert (latency > minPeriod);

			auto random = this->getRandom();

			juce::Array<FloatType> randomSamples;

			for (auto r = 0; r < this->getNumTestingRepetitions(); ++r)
			{
				const auto realPeriod = random.nextInt ({ minPeriod, latency / 2 });

				randomSamples.clearQuick();

				for (auto s = 0; s <= realPeriod; ++s)
					randomSamples.set (s, static_cast<FloatType> (juce::jmap (random.nextFloat(), -1.f, 1.f)));

				for (auto writeIdx = 0, readIdx = 0; writeIdx < storage.getNumSamples(); ++writeIdx)
				{
					storage.setSample (0, writeIdx, randomSamples.getUnchecked (readIdx++));

					if (readIdx > realPeriod)
						readIdx = 0;
				}

				detector.reset();

				this->expectWithinAbsoluteError (detector.detectPeriod (storage),
					static_cast<float> (realPeriod), 1.5f);
			}
		}

		{
			const auto subtest = this->beginSubtest ("Detect random noise as unpitched");

			for (auto r = 0; r < this->getNumTestingRepetitions(); ++r)
			{
				fillAudioBufferWithRandomNoise (storage, this->getRandom());

				detector.reset();

				this->expectEquals (detector.detectPitch (storage), 0.f);
			}
		}

		{
			const auto subtest = this->beginSubtest ("Detect silence as unpitched");

			storage.clear();

			for (auto r = 0; r < this->getNumTestingRepetitions(); ++r)
			{
				detector.reset();
				this->expectEquals (detector.detectPitch (storage), 0.f);
			}
		}

		{
			const auto subtest = this->beginSubtest ("Audio samples");

			for (const auto& sample : binary::getAudioFileNames())
			{
				auto file = binary::getAudioFile (sample);

				this->expect (file.isValid());

				detector.reset();

				detector.setSamplerate (file.getSamplerate());

				const auto midiPitch = sample.fromLastOccurrenceOf ("_", false, false).upToFirstOccurrenceOf (".", false, false).getIntValue();

				this->expectWithinAbsoluteError (math::freqToMidi (detector.detectPitch (file.getData<FloatType>())),
					static_cast<float> (midiPitch), 0.75f);
			}
		}
	}

	static constexpr auto minDetectableFreq = 30.f, maxDetectableFreq = 3000.f;

	AudioBuffer<FloatType> storage;

	dsp::psola::PitchDetector<FloatType> detector { juce::roundToInt (minDetectableFreq) };
};

template struct PitchDetectorTests<float>;
template struct PitchDetectorTests<double>;

LEMONS_CREATE_DSP_TEST (PitchDetectorTests)

}  // namespace lemons::tests
