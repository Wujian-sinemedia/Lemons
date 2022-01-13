#if ! LEMONS_UNIT_TESTS
#  error
#endif

namespace lemons::tests
{

template <typename SampleType>
class PsolaTests final : public AllOscillatorsTest<SampleType>
{
public:
	explicit PsolaTests()
	    : AllOscillatorsTest<SampleType> ("PSOLA tests")
	{
	}

private:
	void runOscillatorTest (dsp::Oscillator<SampleType>& osc) final
	{
		for (const auto samplerate : this->getTestingSamplerates())
		{
			const auto srSubtest = this->beginSubtest ("Samplerate: " + String (samplerate));

			analyzer.releaseResources();

			const auto latency = analyzer.setSamplerate (samplerate);

			const auto detectorLatency = detector.setSamplerate (samplerate);

			this->expectEquals (detectorLatency, latency);

			origAudio.setSize (1, latency);
			shiftedAudio.setSize (1, latency);

			constexpr auto origFreq = SampleType (440);

			osc.setFrequency (origFreq, static_cast<SampleType> (samplerate));

			{
				const auto st = this->beginSubtest ("Shifting down");

				for (const auto ratio : { 0.75, 0.8, 0.9 })
				{
					const auto subtest = this->beginSubtest ("Shifting ratio: " + String (ratio));

					const auto targetPitch = origFreq * static_cast<SampleType> (ratio);

					osc.getSamples (origAudio);

					analyzer.analyzeInput (origAudio);

					shifter.setPitch (juce::roundToInt (targetPitch));

					shifter.getSamples (shiftedAudio);

					this->expectWithinAbsoluteError (detector.detectPitch (shiftedAudio),
					                                 static_cast<float> (targetPitch),
					                                 7.f);
				}
			}

			{
				const auto st = this->beginSubtest ("No shifting");

				osc.getSamples (origAudio);

				analyzer.analyzeInput (origAudio);

				shifter.setPitch (juce::roundToInt (origFreq));

				shifter.getSamples (shiftedAudio);

				this->expectWithinAbsoluteError (detector.detectPitch (shiftedAudio),
				                                 static_cast<float> (origFreq),
				                                 7.f);
			}

			{
				const auto st = this->beginSubtest ("Shifting up");

				for (const auto ratio : { 1.3, 1.1, 1.2 })
				{
					const auto subtest = this->beginSubtest ("Shifting ratio: " + String (ratio));

					const auto targetPitch = origFreq * static_cast<SampleType> (ratio);

					osc.getSamples (origAudio);

					analyzer.analyzeInput (origAudio);

					shifter.setPitch (juce::roundToInt (targetPitch));

					shifter.getSamples (shiftedAudio);

					this->expectWithinAbsoluteError (detector.detectPitch (shiftedAudio),
					                                 static_cast<float> (targetPitch),
					                                 20.f);
				}
			}
		}
	}

	void runOtherTests() final
	{
	}

	dsp::psola::Analyzer<SampleType> analyzer;
	dsp::psola::Shifter<SampleType>  shifter { analyzer };

	AudioBuffer<SampleType> origAudio, shiftedAudio;

	dsp::psola::PitchDetector<SampleType> detector;
};

template class PsolaTests<float>;
template class PsolaTests<double>;

LEMONS_CREATE_DSP_TEST (PsolaTests)

}  // namespace lemons::tests
