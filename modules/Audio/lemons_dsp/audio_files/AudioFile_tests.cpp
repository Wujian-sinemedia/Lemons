#if ! LEMONS_UNIT_TESTS
#  error
#endif

namespace lemons::tests
{

struct AudioFileTests final : public DspTest
{
	explicit AudioFileTests()
		: DspTest ("Audio files")
	{
	}

private:

	void runTest() final
	{
		beginTest ("Loading audio files from binary data");

		const juce::StringArray audioNames { "deke.png", "lemons.jpg", "logo.png" };

		for (const auto filename : audioNames)
		{
			auto file = binary::getAudioFile (filename);

			expect (file.isValid());

			const auto& floatData = file.getData<float>();

			expect (! bufferIsSilent (floatData));

			const auto& doubleData = file.getData<double>();

			expect (! bufferIsSilent (doubleData));
		}

		{
			const auto subtest = beginSubtest ("Getting image file names");

			const auto audioNames_ = binary::getAudioFileNames();

			expect (audioNames_.size() == audioNames.size());

			for (const auto& name : audioNames)
				expect (audioNames_.contains (name));
		}
	}
};

// static AudioFileTests audioFileTest;

}  // namespace lemons::tests
