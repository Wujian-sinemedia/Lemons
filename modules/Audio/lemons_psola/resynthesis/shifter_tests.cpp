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

			origAudio.setSize (1, latency);
			shiftedAudio.setSize (1, latency);

			constexpr auto origFreq = SampleType (440);

			osc.setFrequency (origFreq, static_cast<SampleType> (samplerate));

			auto test_pitch = [&] (SampleType targetPitch)
			{
				osc.getSamples (origAudio);

				analyzer.analyzeInput (origAudio);

				shifter.setPitchHz (juce::roundToInt (targetPitch));

				shifter.getSamples (shiftedAudio);

				this->expect (! bufferIsSilent (shiftedAudio));
				this->expect (! buffersAreEqual (origAudio, shiftedAudio));
			};

			{
				const auto st = this->beginSubtest ("Shifting down");

				for (const auto ratio : { 0.75, 0.8, 0.9 })
				{
					const auto subtest = this->beginSubtest ("Shifting ratio: " + String (ratio));

					const auto targetPitch = origFreq * static_cast<SampleType> (ratio);

					test_pitch (targetPitch);
				}
			}

			{
				const auto st = this->beginSubtest ("No shifting");

				test_pitch (origFreq);
			}

			{
				const auto st = this->beginSubtest ("Shifting up");

				for (const auto ratio : { 1.3, 1.1, 1.2 })
				{
					const auto subtest = this->beginSubtest ("Shifting ratio: " + String (ratio));

					const auto targetPitch = origFreq * static_cast<SampleType> (ratio);

					test_pitch (targetPitch);
				}
			}
		}
	}


	dsp::psola::Analyzer<SampleType> analyzer;
	dsp::psola::Shifter<SampleType>	 shifter { analyzer };

	AudioBuffer<SampleType> origAudio, shiftedAudio;
};

template class PsolaTests<float>;
template class PsolaTests<double>;

LEMONS_CREATE_DSP_TEST (PsolaTests)

}  // namespace lemons::tests
