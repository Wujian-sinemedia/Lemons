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
}

template <typename SampleType>
float PitchDetector<SampleType>::detectPitch (const AudioBuffer<SampleType>& inputAudio)
{
	return detectPitch (inputAudio.getReadPointer (0), inputAudio.getNumSamples());
}

template <typename SampleType>
float PitchDetector<SampleType>::detectPitch (const SampleType* inputAudio, int numSamples)
{
	const auto period = detectPeriod (inputAudio, numSamples);

	if (period > 0.f)
		return math::freqFromPeriod (samplerate, period);

	return 0.f;
}

template <typename SampleType>
float PitchDetector<SampleType>::detectPeriod (const AudioBuffer<SampleType>& inputAudio)
{
	return detectPeriod (inputAudio.getReadPointer (0), inputAudio.getNumSamples());
}

template <typename SampleType>
float PitchDetector<SampleType>::detectPeriod (const SampleType* inputAudio, int numSamples)
{
	jassert (samplerate > 0);                     // pitch detector hasn't been prepared before calling this function!
	jassert (numSamples >= getLatencySamples());  // not enough samples in this frame to do analysis

	updatePeriodBounds();

	auto* yinData = yinBuffer.getWritePointer (0);

	juce::FloatVectorOperations::fill (yinData, SampleType (1), yinBuffer.getNumSamples());

	const auto halfNumSamples = juce::roundToInt (std::floor (numSamples * 0.5f));

	jassert (yinBuffer.getNumSamples() >= halfNumSamples);

	{
		SampleType runningSum = 0;

		for (auto tau = minPeriod; tau <= maxPeriod; ++tau)
		{
			const auto yinIdx = tau - minPeriod;

			// difference function
			for (auto i = 0; i < halfNumSamples; ++i)
			{
				const auto delta = inputAudio[i] - inputAudio[i + tau];
				yinData[yinIdx] += (delta * delta);
			}

			// cumulative mean normalized difference
			runningSum += yinData[yinIdx];
			jassert (runningSum > SampleType (0));
			yinData[yinIdx] *= (static_cast<SampleType> (tau) / runningSum);
		}
	}

	const auto periodEstimate = absoluteThreshold();

	if (periodEstimate > 0)
	{
		periodLastFrame = parabolicInterpolation (periodEstimate) + static_cast<float> (minPeriod);

		jassert (periodLastFrame >= minPeriod && periodLastFrame <= maxPeriod);

		[[maybe_unused]] const auto detectedFreq = math::freqFromPeriod (samplerate, periodLastFrame);

		jassert (detectedFreq >= minHz && detectedFreq <= maxHz);

		return periodLastFrame;
	}

	periodLastFrame = 0.f;
	return 0.f;
}

template <typename SampleType>
void PitchDetector<SampleType>::updatePeriodBounds()
{
	// Pitch should not halve or double between consecutive pitched frames

	const auto periodUpperBound = math::periodInSamples (samplerate, minHz);
	const auto periodLowerBound = math::periodInSamples (samplerate, maxHz);

	if (periodLastFrame > 0.f)
	{
		const auto freqLastFrame = math::freqFromPeriod (samplerate, periodLastFrame);

		maxPeriod = std::min (periodUpperBound, math::periodInSamples (samplerate, freqLastFrame * 0.5f));
		minPeriod = std::max (periodLowerBound, math::periodInSamples (samplerate, freqLastFrame * 2.f));
	}
	else
	{
		maxPeriod = periodUpperBound;
		minPeriod = periodLowerBound;
	}

	jassert (maxPeriod > minPeriod);
	jassert (minPeriod > 0);
}

template <typename SampleType>
int PitchDetector<SampleType>::absoluteThreshold() const
{
	const auto* yinData = yinBuffer.getReadPointer (0);

	const auto tau = [&]() -> int
	{
		for (int tau = 0; tau <= maxPeriod; ++tau)
		{
			if (yinData[tau] < confidenceThresh)
			{
				while (tau + 1 < maxPeriod && yinData[tau + 1] < yinData[tau])
				{
					++tau;
				}

				return tau;
			}
		}
	}();

	if (tau >= maxPeriod || yinData[tau] >= confidenceThresh)
		return 0;

	return tau;
}

template <typename SampleType>
float PitchDetector<SampleType>::parabolicInterpolation (int periodEstimate) const
{
	jassert (periodEstimate > 0);

	const auto x0 = [&]() -> int
	{
		if (periodEstimate < 1)
			return periodEstimate;

		return periodEstimate - 1;
	}();

	const auto x2 = [&]() -> int
	{
		if (const auto plusOne = periodEstimate + 1;
		    plusOne < maxPeriod)
			return plusOne;

		return periodEstimate;
	}();

	const auto* yinData = yinBuffer.getReadPointer (0);

	if (x0 == periodEstimate)
	{
		if (yinData[periodEstimate] > yinData[x2])
			return static_cast<float> (x2);

		return static_cast<float> (periodEstimate);
	}

	if (x2 == periodEstimate)
	{
		if (yinData[periodEstimate] > yinData[x0])
			return static_cast<float> (x0);

		return static_cast<float> (periodEstimate);
	}

	const auto s0 = yinData[x0];
	const auto s2 = yinData[x2];

	const auto period = static_cast<SampleType> (periodEstimate) + (s2 - s0) / (SampleType (2) * (SampleType (2) * yinData[periodEstimate] - s2 - s0));

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
	jassert (newMinHz < maxHz);

	minHz = newMinHz;

	if (samplerate > 0.)
		return setSamplerate (samplerate);

	return 512;
}

template <typename SampleType>
void PitchDetector<SampleType>::setMaxHz (int newMaxHz)
{
	jassert (newMaxHz > minHz);

	maxHz = newMaxHz;
}

template <typename SampleType>
int PitchDetector<SampleType>::getMinHz() const noexcept
{
	return minHz;
}

template <typename SampleType>
int PitchDetector<SampleType>::getMaxHz() const noexcept
{
	return maxHz;
}

template <typename SampleType>
int PitchDetector<SampleType>::getLatencySamples() const noexcept
{
	if (samplerate == 0)
		return 512;

	return math::periodInSamples (samplerate, minHz) * 2;
}

template <typename SampleType>
void PitchDetector<SampleType>::reset()
{
	periodLastFrame = 0.f;
	minPeriod       = 0;
	maxPeriod       = 0;
}

template <typename SampleType>
void PitchDetector<SampleType>::releaseResources()
{
	reset();

	samplerate = 0.;
	yinBuffer.setSize (0, 0);
}

template <typename SampleType>
juce::Range<int> PitchDetector<SampleType>::getCurrentLegalPeriodRange() const
{
	// Did you call one of the pitch detection functions first?
	jassert (maxPeriod > minPeriod);

	return { minPeriod, maxPeriod };
}

template class PitchDetector<float>;
template class PitchDetector<double>;

}  // namespace lemons::dsp::psola
