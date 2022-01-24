#if ! LEMONS_UNIT_TESTS
#  error
#endif

namespace lemons::tests
{

template <typename FloatType>
struct CircularBufferTests final : public DspTest
{
public:
	explicit CircularBufferTests()
		: DspTest (getDspTestName<FloatType> ("Circular buffer tests"))
	{
	}

private:
	void runTest() final
	{
		for (const auto numSamples : getTestingBlockSizes())
		{
			beginTest ("Blocksize: " + String (numSamples));

			resizeAllBuffers (numSamples);

			fillAudioBufferWithRandomNoise (origStorage, getRandom());

			expect (noSamplesAreClipping (origStorage));

			circularBuffer.storeSamples (origStorage);

			expectEquals (circularBuffer.getNumStoredSamples(), numSamples);

			circularBuffer.getSamples (circOutput);

			expectEquals (circularBuffer.getNumStoredSamples(), 0);
			expect (allSamplesAreValid (circOutput));
			expect (noSamplesAreClipping (circOutput));

			expect (allSamplesAreEqual (circOutput, origStorage, 0, numSamples));

			const auto halfNumSamples = numSamples / 2;

			{
				const auto subtest = beginSubtest ("Retrieve fewer samples than were passed in");

				circularBuffer.storeSamples (origStorage);

				auto alias = dsp::buffers::getAliasBuffer (circOutput, 0, halfNumSamples);

				circularBuffer.getSamples (alias);

				expect (allSamplesAreEqual (alias, origStorage, 0, halfNumSamples));
			}

			{
				const auto subtest = beginSubtest ("Retrieve more samples than are left in circ buffer");

				if (math::numberIsEven (numSamples))
					expectEquals (circularBuffer.getNumStoredSamples(), halfNumSamples);
				else
					expectWithinAbsoluteError (circularBuffer.getNumStoredSamples(), halfNumSamples, 1);

				circularBuffer.getSamples (circOutput);

				expect (allSamplesAreZero (circOutput, 0, halfNumSamples));
				expect (allSamplesAreEqual (circOutput, origStorage, halfNumSamples, halfNumSamples));
			}

			const auto subtest = beginSubtest ("Resizing");

			circularBuffer.storeSamples (origStorage);

			circularBuffer.resize (halfNumSamples);

			expectEquals (circularBuffer.getCapacity(), halfNumSamples);
			expectEquals (circularBuffer.getNumStoredSamples(), 0);
		}
	}

	void resizeAllBuffers (int newSize)
	{
		circularBuffer.resize (newSize);
		origStorage.setSize (1, newSize, true, true, true);
		circOutput.setSize (1, newSize, true, true, true);
	}

	AudioBuffer<FloatType> origStorage, circOutput;

	dsp::CircularBuffer<FloatType> circularBuffer;
};

template struct CircularBufferTests<float>;
template struct CircularBufferTests<double>;

LEMONS_CREATE_DSP_TEST (CircularBufferTests)

}  // namespace lemons::tests
