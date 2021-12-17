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


#pragma once

namespace lemons::dsp
{
/** A pitch detector based on the YIN algorithm.
 */
template <typename SampleType>
class PitchDetector final
{
public:
	/** Creates a pitch detector with an initial minimum detectable frequency and confidence threshold.
	    The latency of the algorithm is determined by 2 * the period of the minimum frequency. Therefore, pitch detectors with a higher minimum frequency will have a lower latency.
	    @param minFreqHz The minimum frequency that this pitch detector will be able to detect.
	    @param confidenceThreshold The initial YIN confidence threshold for the detector. See the setConfidenceThresh() method for more documentation on this.
	    @see getLatencySamples(), setConfidenceThresh(), setMinHz()
	 */
	explicit PitchDetector (int minFreqHz = 60, float confidenceThreshold = 0.15f);

	/** Destructor. */
	~PitchDetector() = default;

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


	/** Detects the period, in samples, for a frame of audio.
	    @return The period, in samples, of the fundamental frequency for this frame of audio, or 0.f if the frame is unpitched.
	 */
	[[nodiscard]] float detectPeriod (const AudioBuffer<SampleType>& inputAudio);

	/** Detects the period, in samples, for a frame of audio.
	    @return The period, in samples, of the fundamental frequency for this frame of audio, or 0.f if the frame is unpitched.
	 */
	[[nodiscard]] float detectPeriod (const SampleType* inputAudio, int numSamples);


	/** Returns the latency in samples of the detection algorithm.
	    The latency is equal to 2 * the period of the lowest detectable frequency. Therefore, pitch detectors with a higher minimum frequency will have a lower latency.
	    @see setMinHz()
	 */
	[[nodiscard]] int getLatencySamples() const noexcept;

	/** Sets the samplerate in Hz of the pitch detector.
	    @return The latency, in samples, of the pitch detection algorithm at the new samplerate.
	    @see getLatencySamples()
	 */
	int setSamplerate (double newSamplerate);

	/** Sets the minimum detectable frequency for the pitch detector.
	    The latency of the algorithm is determined by 2 * the period of the minimum frequency. Therefore, pitch detectors with a higher minimum frequency will have a lower latency.
	    @return The latency, in samples, of the pitch detection algorithm with the new minimum frequency.
	 */
	int setMinHz (int newMinHz);

	/** Sets the confidence threshold of the pitch detection algorithm.
	    This value should be between 0 and 1, inclusive, and can be thought of as the amount of aperiodic power tolerable in a signal determined to be pitched.
	    In other words, the lower this value is, the "stricter" the pitch detector is in choosing its pitch estimates.
	    When this value is 1, the pitch detector might output a "pitch" value for a frame of random noise; when this value is 0, the pitch detector might say that even a perfect, pure sine wave is "unpitched".
	    Typical recommended values are between 0.1 and 0.2.
	 */
	void setConfidenceThresh (float newThresh) noexcept;

private:
	inline void cumulativeMeanNormalizedDifference (int halfNumSamples);

	inline int absoluteThreshold (int halfNumSamples) const;

	inline float parabolicInterpolation (int periodEstimate, int halfNumSamples) const;

	int minHz { 60 };

	double samplerate { 0.0 };

	SampleType confidenceThresh { 0.15 };

	AudioBuffer<SampleType> yinBuffer { 1, 512 };

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PitchDetector)
};

}  // namespace lemons::dsp


/*---------------------------------------------------------------------------------------------------------------------------------*/


#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

template <typename FloatType>
struct PitchDetectorTests : public DspTest
{
public:
	PitchDetectorTests();

private:
	void runTest() final;

	void runOscillatorTest (dsp::osc::Oscillator<FloatType>& osc, double samplerate, const String& waveName);

	static const std::vector<float> getConfidenceThresholdsToTest();

	static constexpr auto minDetectableFreq = 30.f;
	static constexpr auto maxDetectableFreq = 3000.f;

	AudioBuffer<FloatType> storage;

	dsp::PitchDetector<FloatType> detector { juce::roundToInt (minDetectableFreq) };

	dsp::osc::Sine<FloatType>     sine;
	dsp::osc::Saw<FloatType>      saw;
	dsp::osc::Square<FloatType>   square;
	dsp::osc::Triangle<FloatType> triangle;
	dsp::osc::SuperSaw<FloatType> superSaw;
};

LEMONS_CREATE_DSP_TEST (PitchDetectorTests)

}  // namespace lemons::tests

#endif
