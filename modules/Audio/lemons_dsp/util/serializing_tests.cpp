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

struct DspSerializingTests final : public CoreTest
{
	explicit DspSerializingTests()
		: CoreTest ("DSP serializing")
	{
	}

private:

	void runTest() final
	{
		// PluginDescription

		auto rand = getRandom();

		{
			const auto subtest = beginSubtest ("ADSR::Parameters");

			const juce::ADSR::Parameters params { rand.nextFloat(), rand.nextFloat(), rand.nextFloat(), rand.nextFloat() };

			const auto decoded = toVarAndBack (params);

			expectEquals (decoded.attack, params.attack);
			expectEquals (decoded.decay, params.decay);
			expectEquals (decoded.sustain, params.sustain);
			expectEquals (decoded.release, params.release);
		}

		{
			const auto subtest = beginSubtest ("Audio buffers");

			runAudioBufferTest<float>();
			runAudioBufferTest<double>();
		}
	}

	template <typename SampleType>
	void runAudioBufferTest()
	{
		const auto subtest = beginSubtest (getPrecisionString<SampleType>() + " precision tests");

		constexpr auto numChannels = 2;
		constexpr auto numSamples  = 512;

		AudioBuffer<SampleType> origAudio { numChannels, numSamples };

		fillAudioBufferWithRandomNoise (origAudio, getRandom());

		//    expect (buffersAreReasonablyEqual (origAudio, toVarAndBack (origAudio)));

		//    const auto block   = serializing::audioToBinary (origAudio);
		//    const auto decoded = serializing::audioFromBinary<SampleType> (block);
		//
		//    expectEquals (decoded.getNumChannels(), numChannels);
		//    expectEquals (decoded.getNumSamples(), numSamples);
		//
		//    expect (buffersAreReasonablyEqual (origAudio, decoded));
	}
};

static DspSerializingTests dspSerializingTest;

}  // namespace lemons::tests
