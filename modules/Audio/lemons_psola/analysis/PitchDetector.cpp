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


namespace lemons::dsp::psola
{

template <typename SampleType>
PitchDetector<SampleType>::PitchDetector (int minFreqHz, float confidenceThreshold)
    : minHz (minFreqHz)
    , confidenceThresh (static_cast<SampleType> (confidenceThreshold))
{
	jassert (minHz > 0);
	jassert (confidenceThresh >= 0 && confidenceThresh <= 1);
}

template <typename SampleType>
[[nodiscard]] float PitchDetector<SampleType>::detectPitch (const AudioBuffer<SampleType>& inputAudio)
{
	return detectPitch (inputAudio.getReadPointer (0), inputAudio.getNumSamples());
}

template <typename SampleType>
[[nodiscard]] float PitchDetector<SampleType>::detectPitch (const SampleType* inputAudio, int numSamples)
{
	const auto period = detectPeriod (inputAudio, numSamples);

	if (period > 0.f)
		return math::freqFromPeriod (samplerate, period);

	return 0.f;
}

template <typename SampleType>
[[nodiscard]] float PitchDetector<SampleType>::detectPeriod (const AudioBuffer<SampleType>& inputAudio)
{
	return detectPeriod (inputAudio.getReadPointer (0), inputAudio.getNumSamples());
}

template <typename SampleType>
[[nodiscard]] float PitchDetector<SampleType>::detectPeriod (const SampleType* inputAudio, int numSamples)
{
	jassert (samplerate > 0);                     // pitch detector hasn't been prepared before calling this function!
	jassert (numSamples >= getLatencySamples());  // not enough samples in this frame to do analysis

	const auto halfNumSamples = juce::roundToInt (std::floor (numSamples * 0.5f));

	jassert (yinBuffer.getNumSamples() >= halfNumSamples);

	auto* yinData = yinBuffer.getWritePointer (0);

	juce::FloatVectorOperations::fill (yinData, SampleType (1), halfNumSamples);

	// difference function
	for (auto tau = 0; tau < halfNumSamples; ++tau)
	{
		for (auto i = 0; i < halfNumSamples; ++i)
		{
			const auto delta = inputAudio[i] - inputAudio[i + tau];
			yinData[tau] += (delta * delta);
		}
	}

	yinData[0] = SampleType (1);

	cumulativeMeanNormalizedDifference (halfNumSamples);

	const auto periodEstimate = absoluteThreshold (halfNumSamples);

	if (periodEstimate <= 0)
		return 0.f;

	return parabolicInterpolation (periodEstimate, halfNumSamples);
}

template <typename SampleType>
inline void PitchDetector<SampleType>::cumulativeMeanNormalizedDifference (int halfNumSamples)
{
	jassert (halfNumSamples > 0);

	auto* yinData = yinBuffer.getWritePointer (0);

	SampleType runningSum = 0;

	for (auto tau = 1; tau < halfNumSamples; ++tau)
	{
		runningSum += yinData[tau];
		yinData[tau] *= (static_cast<SampleType> (tau) / runningSum);
	}
}

template <typename SampleType>
inline int PitchDetector<SampleType>::absoluteThreshold (int halfNumSamples) const
{
	jassert (halfNumSamples > 0);

	const auto* yinData = yinBuffer.getReadPointer (0);

	int tau = 2;

	while (tau < halfNumSamples)
	{
		if (yinData[tau] < confidenceThresh)
		{
			while (tau + 1 < halfNumSamples && yinData[tau + 1] < yinData[tau])
			{
				++tau;
			}

			break;
		}

		++tau;
	}

	if (tau == halfNumSamples || yinData[tau] >= confidenceThresh)
		return -1;

	return tau;
}

template <typename SampleType>
inline float PitchDetector<SampleType>::parabolicInterpolation (int periodEstimate, int halfNumSamples) const
{
	jassert (periodEstimate > 0 && halfNumSamples > 0);

	const auto x0 = periodEstimate < 1 ? periodEstimate : periodEstimate - 1;
	const auto x2 = periodEstimate + 1 < halfNumSamples ? periodEstimate + 1 : periodEstimate;

	const auto* yinData = yinBuffer.getReadPointer (0);

	if (x0 == periodEstimate)
	{
		if (yinData[periodEstimate] <= yinData[x2])
			return static_cast<float> (periodEstimate);

		return static_cast<float> (x2);
	}

	if (x2 == periodEstimate)
	{
		if (yinData[periodEstimate] <= yinData[x0])
			return static_cast<float> (periodEstimate);

		return static_cast<float> (x0);
	}

	const auto s0 = yinData[x0];
	const auto s1 = yinData[periodEstimate];
	const auto s2 = yinData[x2];

	const auto period = static_cast<SampleType> (periodEstimate) + (s2 - s0) / (SampleType (2) * (SampleType (2) * s1 - s2 - s0));

	return static_cast<float> (period);
}

template <typename SampleType>
void PitchDetector<SampleType>::setConfidenceThresh (float newThresh) noexcept
{
	jassert (newThresh >= 0.f && newThresh <= 1.f);
	confidenceThresh = static_cast<SampleType> (newThresh);
}

template <typename SampleType>
int PitchDetector<SampleType>::setSamplerate (double newSamplerate)
{
	jassert (newSamplerate > 0.);

	samplerate = newSamplerate;

	const auto latency = getLatencySamples();

	yinBuffer.setSize (1, (latency / 2) + 1, true, true, true);

	return latency;
}

template <typename SampleType>
int PitchDetector<SampleType>::setMinHz (int newMinHz)
{
	jassert (newMinHz > 0);

	minHz = newMinHz;

	if (samplerate > 0)
		return setSamplerate (samplerate);

	return 512;
}

template <typename SampleType>
int PitchDetector<SampleType>::getMinHz() const noexcept
{
	return minHz;
}

template <typename SampleType>
[[nodiscard]] int PitchDetector<SampleType>::getLatencySamples() const noexcept
{
	if (samplerate == 0)
		return 512;

	return math::periodInSamples (samplerate, minHz) * 2;
}

template class PitchDetector<float>;
template class PitchDetector<double>;

}  // namespace lemons::dsp::psola

