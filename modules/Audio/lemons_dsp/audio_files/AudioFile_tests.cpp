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

		const juce::StringArray audioNames { "vox_60.wav", "vox_67.wav", "vox_72.wav", "inst_84.flac", "inst_60.wav", "inst_79.wav" };

		for (const auto& filename : audioNames)
		{
			auto file = binary::getAudioFile (filename);

			expect (file.isValid());

			const auto& floatData = file.getData<float>();

			expect (! bufferIsSilent (floatData));
			expectEquals (floatData.getNumSamples(), file.getNumSamples());
			expectEquals (floatData.getNumChannels(), file.getNumChannels());

			const auto& doubleData = file.getData<double>();

			expect (! bufferIsSilent (doubleData));
			expectEquals (doubleData.getNumSamples(), file.getNumSamples());
			expectEquals (doubleData.getNumChannels(), file.getNumChannels());

			auto file2 = binary::getAudioFile (filename);

			expectGreaterThan (file.getReferenceCount(), 1);

			file2.duplicateIfShared();

			expect (buffersAreEqual (floatData, file2.getData<float>()));
			expect (buffersAreEqual (doubleData, file2.getData<double>()));
		}

		{
			const auto subtest = beginSubtest ("Getting audio file names");

			const auto audioNames_ = binary::getAudioFileNames();

			expectEquals (audioNames_.size(), audioNames.size());

			for (const auto& name : audioNames)
				expect (audioNames_.contains (name));
		}
	}
};

static AudioFileTests audioFileTest;

}  // namespace lemons::tests
