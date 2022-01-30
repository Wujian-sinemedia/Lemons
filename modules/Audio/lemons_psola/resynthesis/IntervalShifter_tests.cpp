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

template <typename SampleType>
class IntervalShifterTests final : public AllOscillatorsTest<SampleType>
{
public:

	explicit IntervalShifterTests()
		: AllOscillatorsTest<SampleType> ("PSOLA interval shifter tests")
	{
	}

private:

	void runOscillatorTest (dsp::Oscillator<SampleType>& osc) final
	{
		constexpr auto samplerate = 44100.;

		const auto srSubtest = this->beginSubtest ("Samplerate: " + String (samplerate));

		analyzer.releaseResources();

		const auto latency = analyzer.setSamplerate (samplerate);

		origAudio.setSize (1, latency);
		shiftedAudio.setSize (1, latency);

		constexpr auto origFreq = SampleType (440);

		osc.setFrequency (origFreq, static_cast<SampleType> (samplerate));

		const auto origMidi = math::freqToMidi (origFreq);

		auto testInterval = [&] (int interval)
		{
			osc.getSamples (origAudio);

			analyzer.analyzeInput (origAudio);

			shifter.setShiftingInterval (interval);

			shifter.getSamples (shiftedAudio);

			const auto targetPitch = static_cast<float> (math::midiToFreq (origMidi + interval));

			this->expectWithinAbsoluteError (shifter.getPitchHz(), targetPitch, 5.f);
		};

		{
			const auto st = this->beginSubtest ("Shifting down");

			for (const auto interval : { -1, -3, -5, -7, -8, -9, -12 })
			{
				const auto subtest = this->beginSubtest ("Shifting interval: " + String (interval) + " semitones");

				testInterval (interval);
			}
		}

		{
			const auto st = this->beginSubtest ("No shifting");

			testInterval (0);
		}

		{
			const auto st = this->beginSubtest ("Shifting up");

			for (const auto interval : { 1, 3, 5, 7, 8, 9 })
			{
				const auto subtest = this->beginSubtest ("Shifting interval: " + String (interval) + " semitones");

				testInterval (interval);
			}
		}

		{
			const auto st = this->beginSubtest ("Respond to setPitch() changes");

			osc.getSamples (origAudio);

			analyzer.analyzeInput (origAudio);

			constexpr auto interval = 3;

			shifter.setPitchHz (math::midiToFreq (origMidi + interval));

			this->expectEquals (shifter.getShiftingInterval(), interval);
		}
	}

	void runOtherTests() final
	{
	}

	dsp::psola::Analyzer<SampleType>		analyzer;
	dsp::psola::IntervalShifter<SampleType> shifter { analyzer };

	AudioBuffer<SampleType> origAudio, shiftedAudio;
};

template class IntervalShifterTests<float>;
template class IntervalShifterTests<double>;

LEMONS_CREATE_DSP_TEST (IntervalShifterTests)

}  // namespace lemons::tests
