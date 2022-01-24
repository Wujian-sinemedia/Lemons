#if ! LEMONS_UNIT_TESTS
#  error
#endif

namespace lemons::tests
{

template <typename FloatType>
struct AudioFifoTests final : public DspTest
{
public:
	explicit AudioFifoTests()
		: DspTest (getDspTestName<FloatType> ("Audio FIFO tests"))
	{
	}

private:
	void runTest() final
	{
		constexpr auto numChannels = 4;

		for (const auto numSamples : getTestingBlockSizes())
		{
			beginTest ("Blocksize: " + String (numSamples));

			resizeAllBuffers (numSamples, numChannels);

			fillAudioBufferWithRandomNoise (origStorage, getRandom());

			fifo.pushSamples (origStorage);

			logImportantMessage ("Num stored samples stored correctly");

			expectEquals (fifo.numStoredSamples(), numSamples);

			fifo.popSamples (fifoOutput);

			expectEquals (fifo.numStoredSamples(), 0);

			expect (buffersAreEqual (fifoOutput, origStorage));

			const auto halfNumSamples = numSamples / 2;

			using dsp::buffers::getAliasBuffer;

			{
				const auto subtest = beginSubtest ("Retrieve fewer samples than were passed in");

				fifo.pushSamples (origStorage);

				auto outAlias = getAliasBuffer (fifoOutput, 0, halfNumSamples);

				fifo.popSamples (outAlias);

				const auto inAlias = getAliasBuffer (origStorage, 0, halfNumSamples);

				expect (buffersAreEqual (inAlias, outAlias));
			}

			{
				const auto subtest = beginSubtest ("Retrieve more samples than are left in FIFO");

				if (math::numberIsEven (numSamples))
					expectEquals (fifo.numStoredSamples(), halfNumSamples);
				else
					expectWithinAbsoluteError (fifo.numStoredSamples(), halfNumSamples, 1);

				fifo.popSamples (fifoOutput);

				for (int chan = 0; chan < numChannels; ++chan)
					expect (allSamplesAreZero (fifoOutput, 0, halfNumSamples, chan));

				const auto inAlias = getAliasBuffer (origStorage, halfNumSamples, halfNumSamples);

				auto outAlias = getAliasBuffer (fifoOutput, halfNumSamples, halfNumSamples);

				expect (buffersAreEqual (inAlias, outAlias));
			}

			{
				const auto subtest = beginSubtest ("Resizing clears the FIFO");

				fifo.pushSamples (origStorage);

				fifo.resize (halfNumSamples);

				expectEquals (fifo.numStoredSamples(), 0);
			}

			{
				const auto subtest = beginSubtest ("Increase number of channels");

				fifo.resize (halfNumSamples, numChannels + 3);

				expectEquals (fifo.numChannels(), numChannels + 3);
			}

			const auto subtest = beginSubtest ("Decrease number of channels");

			fifo.resize (halfNumSamples, numChannels);

			expectEquals (fifo.numChannels(), numChannels);
		}
	}

	void resizeAllBuffers (int newSize, int numChannels)
	{
		fifo.resize (newSize, numChannels);
		origStorage.setSize (numChannels, newSize, true, true, true);
		fifoOutput.setSize (numChannels, newSize, true, true, true);
	}

	AudioBuffer<FloatType> origStorage, fifoOutput;

	dsp::AudioFifo<FloatType> fifo;
};

template struct AudioFifoTests<float>;
template struct AudioFifoTests<double>;

LEMONS_CREATE_DSP_TEST (AudioFifoTests)

}  // namespace lemons::tests
