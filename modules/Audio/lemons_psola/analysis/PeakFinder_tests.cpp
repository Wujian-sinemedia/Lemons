#if ! LEMONS_UNIT_TESTS
#	error
#endif

namespace lemons::tests
{

template <typename FloatType>
class PeakFinderTests final : public AllOscillatorsTest<FloatType>
{
public:

	explicit PeakFinderTests()
		: AllOscillatorsTest<FloatType> ("PSOLA peak finder")
	{
	}

private:

	static constexpr auto samplerate = 44100.;

	void runOscillatorTest (dsp::Oscillator<FloatType>& osc) final
	{
		for (const auto freq : { 85., 215., 440., 755., 2036., 3531. })
		{
			const auto period = math::periodInSamples (samplerate, freq);

			this->beginTest ("Period: " + String (period) + " samples");

			const auto blocksize = period * 4;

			peakFinder.prepare (blocksize);

			audioStorage.setSize (1, blocksize);

			osc.setFrequency (static_cast<FloatType> (freq),
							  static_cast<FloatType> (samplerate));

			osc.getSamples (audioStorage);

			runBufferTest (period);
		}
	}

	void runOtherTests() final
	{
		const auto numSamples = audioStorage.getNumSamples();

		{
			const auto subtest = this->beginSubtest ("Random noise");

			fillAudioBufferWithRandomNoise (audioStorage, this->getRandom());

			runBufferTest (nextRandomPeriod (samplerate, 60, numSamples));
		}

		{
			const auto subtest = this->beginSubtest ("Silence");

			audioStorage.clear();

			runBufferTest (nextRandomPeriod (samplerate, 60, numSamples));
		}
	}

	void runBufferTest (int period)
	{
		const auto blocksize = audioStorage.getNumSamples();

		const auto& indices = peakFinder.findPeaks (audioStorage.getReadPointer (0), blocksize, period);

		{
			const auto subtest = this->beginSubtest ("Grains identified");
			this->expect (! indices.isEmpty());
			this->expectGreaterOrEqual (indices.size(), (blocksize / period) - 2);
		}

		{
			const auto subtest = this->beginSubtest ("Indices are always increasing");

			for (auto i = 0; i < indices.size() - 1; ++i)
				this->expectGreaterThan (indices.getUnchecked (i + 1),
										 indices.getUnchecked (i));
		}

		{
			const auto subtest = this->beginSubtest ("Max and min index are within range");

			this->expectGreaterOrEqual (indices.getUnchecked (0), 0);

			this->expectLessOrEqual (indices.getUnchecked (indices.size() - 1), blocksize);
		}

		const auto subtest = this->beginSubtest ("Grain spacing");

		/*
		 Heuristics:
		 - grains are approx 2 periods long
		 - grains overlap by approx 50%
		 - grains are centered on points of max energy in signal
		 - individual grain onsets are approx 1 period apart
		 */

		const auto halfPeriod = period / 2;

		for (auto i = 0; i < indices.size() - 2; ++i)
		{
			const auto index1 = indices.getUnchecked (i);
			const auto index2 = indices.getUnchecked (i + 1);
			const auto index3 = indices.getUnchecked (i + 2);

			this->expectWithinAbsoluteError (index3 - index1, period * 2, halfPeriod);

			this->expectWithinAbsoluteError (index2 - index1, period, halfPeriod);
			this->expectWithinAbsoluteError (index3 - index2, period, halfPeriod);
		}
	}

	[[nodiscard]] int nextRandomPeriod (double samplerate, int minHz, int numSamples)
	{
		const auto maxPeriod = std::min (math::periodInSamples (samplerate, minHz), numSamples / 2);
		const auto minPeriod = std::min (numSamples / 4, maxPeriod - 1);

		return this->getRandom().nextInt ({ minPeriod, maxPeriod + 1 });
	}

	dsp::psola::PeakFinder<FloatType> peakFinder;

	AudioBuffer<FloatType> audioStorage;
};

template struct PeakFinderTests<float>;
template struct PeakFinderTests<double>;

// LEMONS_CREATE_DSP_TEST (PeakFinderTests)

}  // namespace lemons::tests
