#pragma once

namespace lemons::dsp
{
/** A pitch detector based on the YIN algorithm.
 */
template <typename SampleType>
class PitchDetector final
{
public:
	/** Creates a pitch detector with a minimum detectable frequency that is constant for the life of the object.
	    The latency of the algorithm is determined by 2 * the period of the minimum frequency. Therefore, pitch detectors with a higher minimum frequency will have a lower latency.
	    @param minFreqHz The minimum frequency that this pitch detector will be able to detect. This value will be constant for the life of this object.
	    @param confidenceThreshold The initial YIN confidence threshold for the detector. See the setConfidenceThresh() method for more documentation on this.
	    @see getLatencySamples(), setConfidenceThresh()
	 */
	explicit PitchDetector (int minFreqHz = 60, float confidenceThreshold = 0.15f);

	/** Returns the latency in samples of the detection algorithm.
	    The latency is equal to 2 * the period of the lowest detectable frequency. Therefore, pitch detectors with a higher minimum frequency will have a lower latency.
	 */
	[[nodiscard]] int getLatencySamples() const noexcept;

	/** Sets the samplerate in Hz of the pitch detector.
	    @return The latency, in samples, of the pitch detection algorithm at the new samplerate.
	    @see getLatencySamples()
	 */
	int setSamplerate (double newSamplerate);

	/** Detects the pitch in Hz for a frame of audio.
	    This can only be used for one channel at a time. If you need to track the pitch of multiple channels of audio, you need one PitchDetector object for each channel.
	    The caller must ensure that there are at least enough samples in this frame of audio for analysis to be performed; ie, that inputAudio.getNumSamples() is greater than or equal to getLatencySamples().
	    @return The pitch in Hz for this frame of audio, or 0.f if the frame is unpitched.
	 */
	[[nodiscard]] float detectPitch (const AudioBuffer<SampleType>& inputAudio);

	/** Detects the pitch in Hz for a frame of audio.
	    @return The pitch in Hz for this frame of audio, or 0.f if the frame is unpitched.
	 */
	[[nodiscard]] float detectPitch (const SampleType* inputAudio, int numSamples);


	/** Sets the confidence threshold of the pitch detection algorithm.
	    This value should be between 0 and 1, inclusive, and can be thought of as the amount of aperiodic power tolerable in a signal determined to be pitched. Typical values are between 0.15 and 0.2.
	 */
	void setConfidenceThresh (float newThresh);

private:
	inline void cumulativeMeanNormalizedDifference (int halfNumSamples);

	inline int absoluteThreshold (int halfNumSamples) const;

	inline float parabolicInterpolation (int periodEstimate, int halfNumSamples) const;

	const int minHz;

	double samplerate { 0.0 };

	SampleType confidenceThresh { 0.15 };

	AudioBuffer<SampleType> yinBuffer { 1, 512 };

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PitchDetector)
};

}  // namespace lemons::dsp


/*---------------------------------------------------------------------------------------------------------------------------------*/


#if LEMONS_UNIT_TESTS

struct PitchDetectorTests : public juce::UnitTest
{
public:
	PitchDetectorTests();

private:
	using FloatType = float;

	void runTest() final;

	lemons::dsp::osc::Sine<FloatType> osc;

	juce::AudioBuffer<FloatType> storage;

	lemons::dsp::PitchDetector<FloatType> detector;
};

static PitchDetectorTests test;

#endif
