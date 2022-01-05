#if ! LEMONS_UNIT_TESTS
#  error
#endif

namespace lemons::tests
{

template <typename FloatType>
struct AudioEngineTests final : public DspTest
{
public:
	explicit AudioEngineTests()
	    : DspTest (getDspTestName<FloatType> ("Audio engine tests"))
	{
	}

private:
	void runTest() final
	{
		constexpr auto numChannels = 3;
		constexpr auto samplerate  = 44100.;

		beginTest ("Latency should be 0");

		expect (! engine.isInitialized());
		expectEquals (engine.reportLatency(), 0);
		expectEquals (engine.getNumChannels(), 0);

		for (const auto blocksize : getTestingBlockSizes())
		{
			beginTest ("Blocksize: " + String (blocksize));

			engine.prepare (samplerate, blocksize, numChannels);
			midiStorage.ensureSize (static_cast<size_t> (blocksize));
			audioIn.setSize (numChannels, blocksize, true, true, true);
			audioOut.setSize (numChannels, blocksize, true, true, true);

			expect (engine.isInitialized());
			expectEquals (engine.getSamplerate(), samplerate);
			expectEquals (engine.getNumChannels(), numChannels);

			fillAudioBufferWithRandomNoise (audioIn, getRandom());
			fillMidiBufferWithRandomEvents (midiStorage, blocksize / 2, getRandom());

			logImportantMessage ("Can call process()");

			engine.process (audioIn);

			expect (allSamplesAreValid (audioIn));

			engine.process (audioIn, midiStorage);

			logImportantMessage ("Alternate bypassed/unbypassed processing");

			audioOut.clear();

			engine.process (audioIn, audioOut, false);
			expect (buffersAreEqual (audioIn, audioOut));

			engine.process (audioIn, audioOut, true);
			expect (! bufferIsSilent (audioOut));
			expect (! buffersAreEqual (audioIn, audioOut));

			engine.process (audioIn, audioOut, true);
			expect (bufferIsSilent (audioOut));

			engine.process (audioIn, audioOut, true);
			expect (bufferIsSilent (audioOut));

			engine.process (audioIn, audioOut, false);
			expect (! bufferIsSilent (audioOut));
			expect (! buffersAreEqual (audioIn, audioOut));

			engine.process (audioIn, audioOut, false);
			expect (buffersAreEqual (audioIn, audioOut));

			engine.process (audioIn, true);
			engine.process (audioIn, false);
			engine.process (audioIn, true);
			engine.process (audioIn, false);
			engine.process (audioIn, true);

			logImportantMessage ("Release");

			engine.releaseResources();

			expect (! engine.isInitialized());
			expectEquals (engine.getSamplerate(), 0.);
			expectEquals (engine.getNumChannels(), 0);
		}
	}

	dsp::PassThroughEngine<FloatType> engine;

	AudioBuffer<FloatType> audioIn, audioOut;

	MidiBuffer midiStorage;
};

template struct AudioEngineTests<float>;
template struct AudioEngineTests<double>;

LEMONS_CREATE_DSP_TEST (AudioEngineTests)

}  // namespace lemons::tests
