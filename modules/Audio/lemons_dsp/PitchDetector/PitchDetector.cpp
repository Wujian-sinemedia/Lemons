#if LEMONS_USE_VDSP
#  include <Accelerate/Accelerate.h>
#endif

namespace lemons::dsp
{
template <typename SampleType>
inline int samplesToFirstZeroCrossing (const SampleType* inputAudio, int numSamples)
{
#if LEMONS_USE_VDSP
	unsigned long index          = 0;
	unsigned long totalcrossings = 0;

	if constexpr (std::is_same_v<SampleType, float>)
		vDSP_nzcros (inputAudio,
		             vDSP_Stride (1),
		             vDSP_Length (1),
		             &index,
		             &totalcrossings,
		             vDSP_Length (numSamples));
	else
		vDSP_nzcrosD (inputAudio,
		              vDSP_Stride (1),
		              vDSP_Length (1),
		              &index,
		              &totalcrossings,
		              vDSP_Length (numSamples));

	return static_cast<int> (index);
#else
	const auto startedPositive = inputAudio[0] > SampleType (0);

	for (int s = 1; s < numSamples; ++s)
		if (startedPositive != (inputAudio[s] > SampleType (0)))
			return s;

	return 0;
#endif
}

template int samplesToFirstZeroCrossing (const float*, int);
template int samplesToFirstZeroCrossing (const double*, int);

template <typename SampleType>
inline void getNextBestPeriodCandidate (juce::Array<int>& candidates, const SampleType* asdfData, int dataSize)
{
	int index = -1;

	for (int i = 0; i < dataSize; ++i)
	{
		if (! candidates.contains (i))
		{
			index = i;
			break;
		}
	}

	if (index == -1) return;

	auto min = asdfData[index];

	for (int i = 0; i < dataSize; ++i)
	{
		if (candidates.contains (i)) continue;

		const auto current = asdfData[i];

		if (current < min)
		{
			min   = current;
			index = i;
		}
	}

	candidates.add (index);
}

template void getNextBestPeriodCandidate (juce::Array<int>&, const float*, int);
template void getNextBestPeriodCandidate (juce::Array<int>&, const double*, int);

static constexpr auto numPeriodCandidatesToTest = 10;

/*-------------------------------------------------------------------------------------*/

template <typename SampleType>
PitchDetector<SampleType>::PitchDetector (int minFreqHz, int maxFreqHz)
    : minHz (minFreqHz)
    , maxHz (maxFreqHz)
{
	jassert (minHz > 0 && maxHz > 0);
	jassert (maxHz > minHz);

	periodCandidates.ensureStorageAllocated (numPeriodCandidatesToTest);
	candidateDeltas.ensureStorageAllocated (numPeriodCandidatesToTest);
	weightedCandidateConfidence.ensureStorageAllocated (numPeriodCandidatesToTest);

	hiCut.prepare();
	loCut.prepare();
}

template <typename SampleType>
typename PitchDetector<SampleType>::FrameLags PitchDetector<SampleType>::getLagsForThisFrame (const SampleType* inputAudio,
                                                                                              int               numSamples,
                                                                                              int               halfNumSamples) const
{
	// the minPeriod & maxPeriod members define the overall global period range; here, the minLag & maxLag local variables are used to define the period range for this specific frame of audio, if it can be constrained more than the global range based on instantaneous conditions

	FrameLags lags;

	// period cannot be smaller than the # of samples to the first zero crossing
	lags.min = samplesToFirstZeroCrossing (inputAudio, numSamples);
	lags.max = halfNumSamples;

	if (lastFrameWasPitched)  // pitch shouldn't halve or double between consecutive voiced frames
	{
		lags.min = std::max (lags.min, juce::roundToInt (lastEstimatedPeriod * SampleType (0.5)));
		lags.max = std::min (lags.max, juce::roundToInt (lastEstimatedPeriod * SampleType (2)));
	}

	lags.min = std::max (lags.min, minPeriod);
	lags.max = std::min (lags.max, maxPeriod);

	if (! (lags.max > lags.min))
		lags.min = std::min (lags.max - 1, minPeriod);

	jassert (lags.max > lags.min);

	return lags;
}

template <typename SampleType>
[[nodiscard]] float PitchDetector<SampleType>::detectPitch (const AudioBuffer<SampleType>& inputAudio)
{
	return detectPitch (inputAudio.getReadPointer (0),
	                    inputAudio.getNumSamples());
}

template <typename SampleType>
[[nodiscard]] float PitchDetector<SampleType>::detectPitch (const SampleType* inputAudio, int numSamples)
{
	jassert (samplerate > 0);  // pitch detector hasn't been prepared before calling this function!

	jassert (numSamples >= 2 * maxPeriod);  // not enough samples in this frame to do analysis

	// TO DO: test if samples are all silent, if so return 0

	const auto halfNumSamples = juce::roundToInt (std::floor (numSamples * 0.5f));

	const auto lags = getLagsForThisFrame (inputAudio, numSamples, halfNumSamples);

	auto* reading = filteringBuffer.getWritePointer (0);

	// copy to filtering buffer
	vecops::copy (inputAudio, reading, numSamples);

	// filter to our min and max possible frequencies
	loCut.coefs.makeHighPass (samplerate,
	                          static_cast<SampleType> (math::freqFromPeriod (samplerate, lags.max)));

	hiCut.coefs.makeLowPass (samplerate,
	                         static_cast<SampleType> (math::freqFromPeriod (samplerate, lags.min)));

	loCut.process (reading, numSamples);
	hiCut.process (reading, numSamples);

	auto* asdfData = asdfBuffer.getWritePointer (0);

	const auto asdfDataSize = lags.max - lags.min + 1;

	jassert (asdfBuffer.getNumSamples() >= asdfDataSize);

	vecops::fill (asdfData, SampleType (0), asdfDataSize);

	for (int k = lags.min; k <= lags.max; ++k)  // k = lag = period
	{
		const auto index = k - lags.min;  // index in asdfBuffer for this datum

		jassert (index >= 0 && index <= asdfDataSize);

		for (int s1 = 0, s2 = halfNumSamples;
		     s1 < halfNumSamples && s2 < numSamples;
		     ++s1, ++s2)
		{
			const auto sample1 = reading[s1] - reading[s1 + k];
			const auto sample2 = reading[s2 - k] - reading[s2];

			const auto difference = sample1 + sample2;

			asdfData[index] += (difference * difference);
		}
	}

	vecops::normalize (asdfData, asdfDataSize);

	int        minIndex           = 0;
	SampleType greatestConfidence = 0;

	vecops::findMinAndMinIndex (asdfData, asdfDataSize, greatestConfidence, minIndex);

	jassert (minIndex >= 0 && minIndex <= asdfDataSize);

	if (greatestConfidence > confidenceThresh)  // determine if frame is unpitched - not enough periodicity
	{
		lastFrameWasPitched = false;
		return 0.f;
	}

	if (lastFrameWasPitched)
		minIndex = chooseIdealPeriodCandidate (asdfData, asdfDataSize, minIndex);

	const auto realPeriod = minIndex + lags.min;

	jassert (realPeriod <= maxPeriod && realPeriod >= minPeriod);

	lastEstimatedPeriod = realPeriod;
	lastFrameWasPitched = true;

	return math::freqFromPeriod (samplerate, realPeriod);
}


template <typename SampleType>
int PitchDetector<SampleType>::chooseIdealPeriodCandidate (
    const SampleType* asdfData, int asdfDataSize, int minIndex)
{
	periodCandidates.clearQuick();
	candidateDeltas.clearQuick();
	weightedCandidateConfidence.clearQuick();

	periodCandidates.add (minIndex);

	for (int c = 1; c <= numPeriodCandidatesToTest; ++c)
		getNextBestPeriodCandidate (periodCandidates, asdfData, asdfDataSize);

	if (periodCandidates.size() <= 2)
		return minIndex;

	// candidate deltas: how far away each period candidate is from the last estimated period

	const auto adding = minPeriod - lastEstimatedPeriod;

	for (auto candidate : periodCandidates)
		candidateDeltas.add (std::abs (candidate + adding));

	// find the difference between the max & min delta values of the candidates
	const auto deltaRange = vecops::findRangeOfExtrema (candidateDeltas);

	if (deltaRange < 2)  // prevent dividing by zero in the next step...
		return minIndex;

	// weight the asdf data based on each candidate's delta value
	// because higher asdf values represent a lower confidence in that period candidate, we want to artificially increase the asdf data a bit for candidates with higher deltas
	for (int c = 0; c < periodCandidates.size(); ++c)
	{
		const auto weighted = asdfData[periodCandidates.getUnchecked (c)]
		                    * (candidateDeltas.getUnchecked (c) / deltaRange);

		weightedCandidateConfidence.add (weighted);
	}

	// choose the estimated period based on the lowest weighted asdf data value

	const auto idx = vecops::findIndexOfMinElement (weightedCandidateConfidence);

	return periodCandidates.getUnchecked (idx);
}

template <typename SampleType>
[[nodiscard]] juce::Range<int> PitchDetector<SampleType>::getCurrentLegalPeriodRange() const
{
	return { minPeriod, maxPeriod };
}

template <typename SampleType>
void PitchDetector<SampleType>::setConfidenceThresh (SampleType newThresh)
{
	confidenceThresh = newThresh;
}

template <typename SampleType>
int PitchDetector<SampleType>::setSamplerate (double newSamplerate)
{
	jassert (newSamplerate > 0);

	if (lastFrameWasPitched)
		lastEstimatedPeriod =
		    juce::roundToInt (newSamplerate / (samplerate / lastEstimatedPeriod));

	samplerate = newSamplerate;

	maxPeriod = juce::roundToInt (samplerate / static_cast<double> (minHz));
	minPeriod = juce::roundToInt (samplerate / static_cast<double> (maxHz));

	if (minPeriod < 1) minPeriod = 1;

	if (! (maxPeriod > minPeriod)) maxPeriod = minPeriod + 1;

	const auto numOfLagValues = maxPeriod - minPeriod + 1;

	asdfBuffer.setSize (1, numOfLagValues, true, true, true);
	filteringBuffer.setSize (1, numOfLagValues, true, true, true);

	hiCut.prepare();
	loCut.prepare();

	return getLatencySamples();
}

template <typename SampleType>
[[nodiscard]] int PitchDetector<SampleType>::getLatencySamples() const noexcept
{
	jassert (maxPeriod > 0);
	return 2 * maxPeriod;
}

template class PitchDetector<float>;
template class PitchDetector<double>;

}  // namespace lemons::dsp


/*---------------------------------------------------------------------------------------------------------------------------------*/


#if LEMONS_UNIT_TESTS

PitchDetectorTests::PitchDetectorTests()
    : juce::UnitTest ("PitchDetectorTests", "DSP")
{
}

using juce::String;

inline String getFailureMessage (float guess, float actual)
{
    return String("Detected incorrect frequency! Estimated MIDI: ")
        + String(lemons::math::freqToMidi(guess))
        + "; Actual MIDI: " + String(lemons::math::freqToMidi(actual));
}

void PitchDetectorTests::runTest()
{
	constexpr auto samplerate = 44100.;

	const auto latency = detector.setSamplerate (samplerate);

	storage.setSize (1, latency);

	constexpr auto correctFreq = 440.f;

	beginTest ("Detect frequency of sine wave");

	osc.setFrequency (correctFreq, samplerate);
	osc.getSamples (storage);

	const auto guess = detector.detectPitch (storage);

	expectEquals (guess, correctFreq,
                  getFailureMessage (guess, correctFreq));
}

#endif
