#if ! LEMONS_UNIT_TESTS
#  error
#endif

namespace lemons::tests
{

template <typename FloatType>
struct MonoStereoConverterTests final : public DspTest
{
public:
	explicit MonoStereoConverterTests()
	    : DspTest (getDspTestName<FloatType> ("Mono-stereo converter tests"))
	{
	}

private:
	void runTest() final
	{
		constexpr auto blocksize = 512;

		converter.prepare (blocksize);

		monoBuffer.setSize (1, blocksize);
		stereoBuffer.setSize (2, blocksize);

		monoBuffer.clear();
		stereoBuffer.clear();

		beginTest ("Mono to stereo");

		fillAudioBufferWithRandomNoise (monoBuffer, getRandom());

		converter.convertMonoToStereo (monoBuffer, stereoBuffer);

		expect (bufferChannelsAreEqual (monoBuffer, 0, stereoBuffer, 0));
		expect (bufferChannelsAreEqual (monoBuffer, 0, stereoBuffer, 1));

		beginTest ("Stereo to mono");

		monoBuffer.clear();
		stereoBuffer.clear();

		fillAudioBufferWithRandomNoise (stereoBuffer, getRandom());

		{
			const auto subtest = beginSubtest ("Left only");

			converter.setStereoReductionMode (dsp::StereoReductionMode::leftOnly);

			converter.convertStereoToMono (stereoBuffer, monoBuffer);

			expect (bufferChannelsAreEqual (monoBuffer, 0, stereoBuffer, 0));
			expect (! bufferChannelsAreEqual (monoBuffer, 0, stereoBuffer, 1));
		}

		{
			const auto subtest = beginSubtest ("Right only");

			converter.setStereoReductionMode (dsp::StereoReductionMode::rightOnly);

			converter.convertStereoToMono (stereoBuffer, monoBuffer);

			expect (bufferChannelsAreEqual (monoBuffer, 0, stereoBuffer, 1));
			expect (! bufferChannelsAreEqual (monoBuffer, 0, stereoBuffer, 0));
		}

		const auto subtest = beginSubtest ("Mix to mono");

		converter.setStereoReductionMode (dsp::StereoReductionMode::mixToMono);

		converter.convertStereoToMono (stereoBuffer, monoBuffer);

		expect (! bufferChannelsAreEqual (monoBuffer, 0, stereoBuffer, 0));
		expect (! bufferChannelsAreEqual (monoBuffer, 0, stereoBuffer, 1));
		expect (noSamplesAreClipping (monoBuffer));
	}

	AudioBuffer<FloatType> monoBuffer, stereoBuffer;

	dsp::MonoStereoConverter<FloatType> converter;
};

template struct MonoStereoConverterTests<float>;
template struct MonoStereoConverterTests<double>;

LEMONS_CREATE_DSP_TEST (MonoStereoConverterTests)

}  // namespace lemons::tests
