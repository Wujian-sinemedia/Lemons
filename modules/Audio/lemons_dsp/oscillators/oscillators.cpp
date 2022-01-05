/*
 ======================================================================================

 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.

 ======================================================================================
 */

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
	jassert (increment > 0);

	if (phase < increment)
	{
		const auto p = phase / increment;
		return (SampleType (2) - p) * p - SampleType (1);
	}

	if (phase > (SampleType (1) - increment))
	{
		const auto p = (phase - SampleType (1)) / increment;
		return (p + SampleType (2)) * p + SampleType (1);
	}

	return SampleType (0);
}

template float  blep (float, float) noexcept;
template double blep (double, double) noexcept;

/*--------------------------------------------------------------------------------------------*/

template <typename SampleType>
Oscillator<SampleType>::Oscillator (std::function<SampleType()>&& sampleFuncToUse)
    : sampleFunc (std::move (sampleFuncToUse))
{
}

template <typename SampleType>
SampleType Oscillator<SampleType>::getSample()
{
	return sampleFunc();
}

template <typename SampleType>
void Oscillator<SampleType>::getSamples (SampleType* output, int numSamples)
{
	for (int i = 0; i < numSamples; ++i)
		output[i] = sampleFunc();
}

template <typename SampleType>
void Oscillator<SampleType>::getSamples (AudioBuffer<SampleType>& output, int channel)
{
	getSamples (output.getWritePointer (channel), output.getNumSamples());
}

template <typename SampleType>
void Oscillator<SampleType>::skipSamples (int numToSkip)
{
	for (int i = 0; i < numToSkip; ++i)
		sampleFunc();
}

template struct Oscillator<float>;
template struct Oscillator<double>;

/*--------------------------------------------------------------------------------------------*/

template <typename SampleType>
Sine<SampleType>::Sine()
    : Oscillator<SampleType> ([&]
                              { return std::sin (phase.next (juce::MathConstants<SampleType>::twoPi)); })
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
	phase.setFrequency (juce::MathConstants<SampleType>::twoPi * frequency, sampleRate);
	freq = frequency;
}

template <typename SampleType>
SampleType Sine<SampleType>::getFrequency() const noexcept
{
	return freq;
}

template struct Sine<float>;
template struct Sine<double>;

/*--------------------------------------------------------------------------------------------*/

template <typename SampleType>
Saw<SampleType>::Saw()
    : Oscillator<SampleType> ([&]
                              {
    const auto p = phase.next (1);
    return SampleType (2) * p - SampleType (1) - blep (p, phase.getIncrement()); })
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
	freq = frequency;
}

template <typename SampleType>
SampleType Saw<SampleType>::getFrequency() const noexcept
{
	return freq;
}

template struct Saw<float>;
template struct Saw<double>;

/*--------------------------------------------------------------------------------------------*/

template <typename SampleType>
Square<SampleType>::Square()
    : Oscillator<SampleType> ([&]
                              {
    const auto inc = phase.getIncrement();
    const auto p   = phase.next (1);
    
    const auto base = p < SampleType (0.5) ? SampleType (-1) : SampleType (1);
    
    return base - blep (p, inc) + blep (std::fmod (p + SampleType (0.5), SampleType (1)), inc); })
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
	freq = frequency;
}

template <typename SampleType>
SampleType Square<SampleType>::getFrequency() const noexcept
{
	return freq;
}

template struct Square<float>;
template struct Square<double>;

/*--------------------------------------------------------------------------------------------*/

template <typename SampleType>
Triangle<SampleType>::Triangle()
    : Oscillator<SampleType> ([&]
                              {
    sum += SampleType (4) * square.phase.getIncrement() * square.getSample();
    return sum; })
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
	freq = frequency;
}

template <typename SampleType>
SampleType Triangle<SampleType>::getFrequency() const noexcept
{
	return freq;
}

template struct Triangle<float>;
template struct Triangle<double>;

}  // namespace lemons::dsp::osc

