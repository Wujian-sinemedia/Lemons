
#pragma once

namespace lemons::dsp::FX
{
/**
	A simple compressor effect class that allows you to optionally sidechain the signal.
	This class is essentially a refactor of the compressor class from the juce dsp module, with the sidechaining capabilities added.
	@see LevelReportingAudioEffect, AudioEffect
 */
template <typename SampleType>
class Compressor : public LevelReportingAudioEffect<SampleType>
{
public:
	/** Creates a compressor with some default initial settings. */
	Compressor();

	/** Creates a compressor with some initial settings. */
	Compressor (float thresh, float ratioToUse, float attackMs, float releaseMs);

	/** Prepares the compressor. */
	void prepare (double samplerate, int blocksize) final;


	/** Processes a single channel of audio.
		@param channel The channel number that this input sample is from, in the original buffer.
		@param numSamples The number of samples to process.
		@param signalToCompress Pointer to contiguous uncompressed input sample values.
		@param sidechain Pointer to contiguous sidechain signal values.
		@return The gain reduction amount for this frame of this channel's audio.
	*/
	SampleType processChannel (int				 channel,
							   int				 numSamples,
							   SampleType*		 signalToCompress,
							   const SampleType* sidechain) final;


	/** Processes a single sample.
		@param channel The channel number that this input sample is from, in the original buffer.
		@param inputSample The value of the uncompressed input sample.
		@param sidechainSample The value of the sidechain sample.
		@param gainReduction Pointer to where to write the gain reduction value for this sample. This may be null.
		@return The output compressed sample.
	*/
	SampleType processSample (int		  channel,
							  SampleType  inputSample,
							  SampleType  sidechainSample,
							  SampleType* gainReduction);

	/** Sets the threshold, in decidebls, of the compressor. */
	void setThreshold (float newThresh_dB);

	/** Sets the compression ratio. */
	void setRatio (float newRatio);

	/** Sets the compressor's attack time in milliseconds. */
	void setAttack (float attackMs);

	/** Sets the compressor's release time in milliseconds. */
	void setRelease (float releaseMs);

	/** Resets the compressor's internal state to a neutral one. */
	void reset();


private:
	void update();

	SampleType								threshold { 0. }, thresholdInverse { 0. }, ratioInverse { 0. };
	juce::dsp::BallisticsFilter<SampleType> envelopeFilter;

	juce::dsp::ProcessSpec spec;

	double	   sampleRate { 44100.0 };
	SampleType thresholddB { 0.0 }, ratio { 1.0 }, attackTime { 1.0 }, releaseTime { 100.0 };
};

}  // namespace lemons::dsp::FX
