
namespace lemons::dsp::osc
{
template <typename SampleType>
void Phase<SampleType>::resetPhase() noexcept
{
	phase = 0;
}

template <typename SampleType>
void Phase<SampleType>::setFrequency (SampleType frequency, SampleType sampleRate)
{
	jassert (sampleRate > 0 && frequency > 0);
	increment = frequency / sampleRate;
}

template <typename SampleType>
SampleType Phase<SampleType>::next (SampleType wrapLimit) noexcept
{
	const auto p = phase;

	phase += increment;
	while (phase >= wrapLimit)
		phase -= wrapLimit;

	return p;
}

template <typename SampleType>
SampleType Phase<SampleType>::getIncrement() const noexcept
{
	return increment;
}

template struct Phase<float>;
template struct Phase<double>;

/*--------------------------------------------------------------------------------------------*/

template <typename SampleType>
static inline SampleType blep (SampleType phase, SampleType increment) noexcept
{
	static constexpr SampleType one = 1;

	if (phase < increment)
	{
		auto p = phase / increment;
		return (2 - p) * p - one;
	}

	if (phase > one - increment)
	{
		auto p = (phase - one) / increment;
		return (p + 2) * p + one;
	}

	return SampleType (0);
}

template float  blep (float phase, float increment) noexcept;
template double blep (double phase, double increment) noexcept;

/*--------------------------------------------------------------------------------------------*/

template <typename SampleType>
void Oscillator<SampleType>::getSamples (SampleType* output, int numSamples)
{
	for (int i = 0; i < numSamples; ++i)
		output[i] = getSample();
}

template <typename SampleType>
void Oscillator<SampleType>::getSamples (juce::AudioBuffer<SampleType>& output, int channel)
{
	getSamples (output.getWritePointer (channel),
	            output.getNumSamples());
}

template <typename SampleType>
void Oscillator<SampleType>::skipSamples (int numToSkip)
{
	for (int i = 0; i < numToSkip; ++i)
		getSample();
}

template struct Oscillator<float>;
template struct Oscillator<double>;

/*--------------------------------------------------------------------------------------------*/

template <typename SampleType>
Sine<SampleType>::Sine()
{
	phase.resetPhase();
}

template <typename SampleType>
void Sine<SampleType>::resetPhase()
{
	phase.resetPhase();
}

template <typename SampleType>
void Sine<SampleType>::setFrequency (SampleType frequency, SampleType sampleRate)
{
	phase.setFrequency (twoPi * frequency, sampleRate);
}

template <typename SampleType>
SampleType Sine<SampleType>::getSample()
{
	return std::sin (phase.next (twoPi));
}

template struct Sine<float>;
template struct Sine<double>;

/*--------------------------------------------------------------------------------------------*/

template <typename SampleType>
Saw<SampleType>::Saw()
{
	phase.resetPhase();
}

template <typename SampleType>
void Saw<SampleType>::resetPhase()
{
	phase.resetPhase();
}

template <typename SampleType>
void Saw<SampleType>::setFrequency (SampleType frequency, SampleType sampleRate)
{
	phase.setFrequency (frequency, sampleRate);
}

template <typename SampleType>
SampleType Saw<SampleType>::getSample()
{
	auto p = phase.next (1);
	return SampleType (2.0) * p - SampleType (1.0) - blep (p, phase.getIncrement());
}

template struct Saw<float>;
template struct Saw<double>;

/*--------------------------------------------------------------------------------------------*/

template <typename SampleType>
Square<SampleType>::Square()
{
	phase.resetPhase();
}

template <typename SampleType>
void Square<SampleType>::resetPhase()
{
	phase.resetPhase();
}

template <typename SampleType>
void Square<SampleType>::setFrequency (SampleType frequency, SampleType sampleRate)
{
	phase.setFrequency (frequency, sampleRate);
}

template <typename SampleType>
SampleType Square<SampleType>::getSample()
{
	const auto inc = phase.getIncrement();
	const auto p   = phase.next (1);

	return (p < SampleType (0.5) ? SampleType (-1) : SampleType (1))
	     - blep (p, inc)
	     + blep (std::fmod (p + SampleType (0.5), SampleType (1)), inc);
}

template struct Square<float>;
template struct Square<double>;

/*--------------------------------------------------------------------------------------------*/

template <typename SampleType>
Triangle<SampleType>::Triangle()
{
	resetPhase();
}

template <typename SampleType>
void Triangle<SampleType>::resetPhase()
{
	square.resetPhase();
	sum = static_cast<SampleType> (1);
}

template <typename SampleType>
void Triangle<SampleType>::setFrequency (SampleType frequency, SampleType sampleRate)
{
	square.setFrequency (frequency, sampleRate);
}

template <typename SampleType>
SampleType Triangle<SampleType>::getSample()
{
	sum += SampleType (4) * square.phase.getIncrement() * square.getSample();
	return sum;
}

template struct Triangle<float>;
template struct Triangle<double>;

}  // namespace lemons::dsp::osc


/*--------------------------------------------------------------------------------------------*/

#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

template <typename SampleType>
OscillatorTests<SampleType>::OscillatorTests()
    : DspTest (getDspTestName<SampleType> ("Oscillator tests"))
{
}

template <typename SampleType>
void OscillatorTests<SampleType>::runTest()
{
	beginTest ("Sine tests");
	runOscillatorTests (sine);

	beginTest ("Saw tests");
	runOscillatorTests (saw);

	beginTest ("Square tests");
	runOscillatorTests (square);

	//	beginTest ("Triangle tests");
	//	runOscillatorTests (triangle);
}

template <typename SampleType>
void OscillatorTests<SampleType>::runOscillatorTests (dsp::osc::Oscillator<SampleType>& osc)
{
	for (const auto samplerate : getTestingSamplerates())
	{
		const auto samplerateSubtest = beginSubtest ("Samplerate: " + String (samplerate));

		for (const auto period : { 50, 119, 350, 531 })
		{
			const auto periodSubtest = beginSubtest ("Period: " + String (period));

			const auto blocksize = period * 4;

			storage.setSize (1, blocksize, true, true, true);
			storage.clear();

			const auto freq = math::freqFromPeriod (samplerate, period);

			logImportantMessage ("Frequency: " + String (freq));

			osc.setFrequency (static_cast<SampleType> (freq),
			                  static_cast<SampleType> (samplerate));

			osc.getSamples (storage);

			expect (! bufferIsSilent (storage));
			expect (allSamplesAreValid (storage));
			expect (noSamplesAreClipping (storage));


			const auto subtest = beginSubtest ("Test zero-crossings");

			const auto* samples = storage.getReadPointer (0);

			zeroCrossings.clearQuick();
			bool status = samples[0] > SampleType (0);

			for (int s = 0; s < blocksize; ++s)
			{
				const auto current = samples[s] > SampleType (0);

				if (current != status)
				{
					zeroCrossings.add (s);
					status = current;
				}
			}

			expectGreaterThan (zeroCrossings.size(), 2);

			for (int i = 1; i < zeroCrossings.size(); ++i)
			{
				expectWithinAbsoluteError (zeroCrossings.getUnchecked (i) - zeroCrossings.getUnchecked (i - 1),
				                           period / 2,
				                           3);
			}
		}
	}
}

template struct OscillatorTests<float>;
template struct OscillatorTests<double>;

}  // namespace lemons::tests

#endif
