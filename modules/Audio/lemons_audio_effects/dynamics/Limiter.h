
#pragma once

namespace lemons::dsp::FX
{
/**
	A simple limiter effect class that allows you to optionally sidechain the signal.
	This class is essentially a refactor of the limiter class from the juce dsp module, with the sidechaining capabilities added.
	@see LevelReportingAudioEffect, AudioEffect
 */
template <typename SampleType>
class Limiter : public LevelReportingAudioEffect<SampleType>
{
public:

	/** Creates a limiter with some default initial settings. */
	Limiter();

	/** Creates a limiter with some initial settings. */
	Limiter (float threshDB, float releaseMs);

	/** Prepares the limiter. */
	void prepare (double samplerate, int blocksize) final;

	/** Sets the threshold, in decibels, of the limiting algorithm. */
	void setThreshold (float thresh_dB);

	/** Sets the release time, in milliseconds, of the limiter. */
	void setRelease (float release_ms);

	/** Resets the limiter's internal state to a neutral one. */
	void reset();


	/** Processes a single channel of audio.
		@param channel The channel number that this input sample is from, in the original buffer.
		@param numSamples The number of samples to process.
		@param signalToLimit Pointer to contiguous input sample values.
		@param sidechain Pointer to contiguous sidechain signal values.
		@return The gain reduction amount for this frame of this channel's audio.
	*/
	SampleType processChannel (int channel,
		int						   numSamples,
		SampleType*				   signalToLimit,
		const SampleType*		   sidechain) final;


private:

	void update();

	Compressor<SampleType> firstStageCompressor, secondStageCompressor;

	juce::SmoothedValue<SampleType, juce::ValueSmoothingTypes::Linear>
		outputVolume;

	double sampleRate { 44100.0 };
	float  thresholddB { -10.0 }, releaseTime { 100.0 };
};

}  // namespace lemons::dsp::FX
