
#pragma once

namespace lemons::dsp
{
/** An ASDF-based pitch detector.
    This pitch detection algorithm works in the time domain to estimate the fundamental frequency of a signal by finding the offset with the highest autocorrelation value -- ie, the period. \n \n
    This algorithm is intended for use with perfectly monophonic signals, and has been most extensively tested with vocals.
 */
template <typename SampleType>
class PitchDetector
{
public:
	/** Creates a pitch detector with a detectable frequency range that is constant for the life of the object.
	    The latency of the algorithm is determined by 2 * the period of the minimum frequency. \n
	    Therefore, pitch detectors with a higher minimum frequency will have a lower latency.
	 */
	PitchDetector (int minFreqHz = 60, int maxFreqHz = 2500);

	/** Returns the latency in samples of the detection algorithm. */
	int getLatencySamples() const noexcept;

	/** Releases the pitch detector's resources. */
	void releaseResources();


	/** Detects the pitch in Hz for a frame of audio.
	    This can only be used for one channel at a time. If you need to track the pitch of multiple channels of audio, you need one PitchDetector object for each channel.
	    @return The pitch in Hz for this frame of audio, or 0.f if the frame is unpitched.
	 */
	float detectPitch (const AudioBuffer<SampleType>& inputAudio);

	/** Detects the pitch in Hz for a frame of audio.
	    @return The pitch in Hz for this frame of audio, or 0.f if the frame is unpitched.
	 */
	float detectPitch (const SampleType* inputAudio, int numSamples);


	/** Sets the confidence threshold of the pitch detection algorithm.
	    This is similar to YIN's post-processing ("peak-picking") concepts; this assures a degree of confidence in the rolling average of chosen most-periodic peaks, to attempt to eliminate octave errors and filter out noise from the results.
	    @param newThresh The amount of periodicity that the most-likely period candidate must have in order for a frame to be determined pitched. \n \n
	    If the highest autocorrelation value (ie, the greatest amount of periodicity) is below this threshold, the frame will be determined to be unpitched. \n \n
	    The value should be between 0. and 1., inclusive; 0 means that nothing will be detected as unpitched, and 1 means that only exactly perfect sine waves are detected as pitched.
	 */
	void setConfidenceThresh (SampleType newThresh);

	/** Sets the samplerate in Hz of the pitch detector. */
	void setSamplerate (double newSamplerate);

	/** Returns a range object representing the current legal range of period values. */
	juce::Range<int> getCurrentLegalPeriodRange() const;

private:
	int chooseIdealPeriodCandidate (const SampleType* asdfData,
	                                int               asdfDataSize,
	                                int               minIndex);

	const int minHz;
	const int maxHz;
	int       minPeriod { 0 }, maxPeriod { 0 };

	int  lastEstimatedPeriod { 0 };
	bool lastFrameWasPitched { false };

	double samplerate { 0.0 };

	SampleType confidenceThresh { static_cast<SampleType> (0.15) };

	AudioBuffer<SampleType> asdfBuffer { 1, 512 };  // calculated ASDF values will be placed in this buffer

	Array<int>        periodCandidates;
	Array<int>        candidateDeltas;
	Array<SampleType> weightedCandidateConfidence;

	AudioBuffer<SampleType>     filteringBuffer { 1, 512 };
	filters::Filter<SampleType> loCut, hiCut;

	static constexpr int numPeriodCandidatesToTest = 10;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PitchDetector)
};

}  // namespace lemons::dsp
