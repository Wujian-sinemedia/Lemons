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

template <typename FloatType>
struct DspProtectorTests final : public DspTest
{
	explicit DspProtectorTests()
		: DspTest (getDspTestName<FloatType> ("DSP protection"))
	{
	}

private:

	void runTest() final
	{
		beginTest ("DSP protection");

		static constexpr auto numSamples  = 512;
		static constexpr auto numChannels = 2;
		static constexpr auto samplerate  = 44100.;

		audioStorage.setSize (numChannels, numSamples);

		protector.prepare (samplerate, numSamples, numChannels);

		audioStorage.clear();

		protector.process (audioStorage);

		expect (! protector.isMuted());

		fillAudioBufferWithRandomNoise (audioStorage, getRandom());

		protector.forceMute();

		protector.process (audioStorage);
		protector.process (audioStorage);

		expect (bufferIsSilent (audioStorage));

		auto test_sample_value_mutes_protector = [&] (FloatType sample)
		{
			protector.resetMute();

			fillAudioBufferWithRandomNoise (audioStorage, getRandom());

			protector.process (audioStorage);

			expect (! bufferIsSilent (audioStorage));

			audioStorage.setSample (0, 23, sample);

			protector.process (audioStorage);

			expect (protector.isMuted());
		};

		test_sample_value_mutes_protector (static_cast<FloatType> (1.0001));
		test_sample_value_mutes_protector (static_cast<FloatType> (-1.0001));

		test_sample_value_mutes_protector (std::numeric_limits<FloatType>::max());
	}

	dsp::Protector<FloatType> protector;

	AudioBuffer<FloatType> audioStorage;
};

template struct DspProtectorTests<float>;
template struct DspProtectorTests<double>;

LEMONS_CREATE_DSP_TEST (DspProtectorTests)

}  // namespace lemons::tests
