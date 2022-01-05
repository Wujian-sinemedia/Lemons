#if ! LEMONS_UNIT_TESTS
#  error
#endif

namespace lemons::tests
{

template <typename FloatType>
struct LatencyEngineTests final : public DspTest
{
public:
	explicit LatencyEngineTests()
	    : DspTest (getDspTestName<FloatType> ("Latency audio engine tests"))
	{
	}

private:
	void runTest() final
	{
		constexpr auto numChannels = 2;
		constexpr auto samplerate  = 44100.;

		beginTest ("Initial latency should be 0");

		expect (! engine.isInitialized());
		expectEquals (engine.reportLatency(), 0);

		for (const auto blocksize : getTestingBlockSizes())
		{
			beginTest ("Blocksize: " + String (blocksize));

			midiStorage.ensureSize (static_cast<size_t> (blocksize));
			audioIn.setSize (numChannels, blocksize, true, true, true);
			audioOut.setSize (numChannels, blocksize, true, true, true);

			fillAudioBufferWithRandomNoise (audioIn, getRandom());
			fillMidiBufferWithRandomEvents (midiStorage, blocksize / 2, getRandom());

			const auto testLatency = [&] (int latency)
			{
				engine.prepare (samplerate, blocksize, numChannels);

				expect (engine.isInitialized());

				engine.changeLatency (latency);

				expectEquals (engine.reportLatency(), latency);

				logImportantMessage ("Can call process()");

				audioOut.clear();

				engine.process (audioIn, audioOut, midiStorage);

				expect (bufferIsSilent (dsp::buffers::getAliasBuffer (audioOut, 0, std::min (latency, audioOut.getNumSamples()))));

				engine.process (audioIn, audioOut, midiStorage);
				engine.process (audioIn, audioOut, midiStorage);

				engine.releaseResources();
			};

			logImportantMessage ("Latency equal to blocksize");
			testLatency (blocksize);

			logImportantMessage ("Latency larger than blocksize");
			testLatency (blocksize * 2);

			logImportantMessage ("Latency smaller than blocksize");
			testLatency (blocksize / 2);
		}
	}

	struct PassThroughEngine : public dsp::LatencyEngine<FloatType>
	{
		void renderChunk (const AudioBuffer<FloatType>& input,
		                  AudioBuffer<FloatType>&       output,
		                  MidiBuffer& midiMessages, bool isBypassed) final
		{
			jassert (input.getNumSamples() == output.getNumSamples());
			jassert (input.getNumChannels() == output.getNumChannels());
			jassert (input.getNumSamples() == this->reportLatency());

			if (isBypassed)
				output.clear();
			else
				dsp::buffers::copy (input, output);
		}
	};

	PassThroughEngine engine;

	AudioBuffer<FloatType> audioIn, audioOut;

	MidiBuffer midiStorage;
};

template struct LatencyEngineTests<float>;
template struct LatencyEngineTests<double>;

LEMONS_CREATE_DSP_TEST (LatencyEngineTests)

}  // namespace lemons::tests
