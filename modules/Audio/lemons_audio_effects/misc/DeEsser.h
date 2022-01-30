/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

#pragma once


namespace lemons::dsp::FX
{
/**
	A basic "de-esser" effect class.
	Mainly intended for removing sibilances from a vocal signal, this works by sending the input signal through an inverted noise gate sidechained by a hi-passed version of the original signal.
	@see filters::MultiFilter, NoiseGate, LevelReportingAudioEffect, AudioEffect
 */
template <typename SampleType>
class DeEsser : public LevelReportingAudioEffect<SampleType>
{
public:

	/** Creates a DeEsser with some default initial settings. */
	DeEsser();

	/** Creates a DeEsser with some specified initial settings. */
	DeEsser (float threshDB, int deEssAmount);

	/** Prepares the DeEsser. */
	void prepare (double samplerate, int blocksize) final;

	/** Resets the DeEsser's internal state to a neutral one. */
	void reset();


	/** Sets the de-essing threshold.
		Specifically, this sets the threshold of the de-esser's internal noise gate that is sidechained by the filtered signal.
		@see NoiseGate::setThreshold()
	*/
	void setThresh (float newThresh_dB);


	/** Sets the de-essing amount (think of it like a percentage knob).
		This should be an integer in the range 0-100 (inclusive). \n \n
		Internally, this controls the ratio of the de-esser's internal noise gate that is sidechained by the filtered signal.
		@see NoiseGate::setRatio()
	*/
	void setDeEssAmount (int newAmount);


	/** Processes a single channel of audio.
		@param channel The channel number that this input sample is from, in the original buffer.
		@param numSamples The number of samples to process.
		@param signalToDeEss Pointer to contiguous input sample values.
		@param sidechain Pointer to contiguous sidechain signal values, if any.
		@return The gain reduction amount for this frame of this channel's audio.
	*/
	SampleType processChannel (int				 channel,
							   int				 numSamples,
							   SampleType*		 signalToDeEss,
							   const SampleType* sidechain) final;


private:

	dsp::filters::MultiFilter<SampleType, 2> filter;

	NoiseGate<SampleType> gate;

	int	  amt { 50 };
	float thresh { -3.f };

	static constexpr double hiPassFreq { 7600. };
	static constexpr int	attackMs { 20 };
	static constexpr int	releaseMs { 30 };
};

}  // namespace lemons::dsp::FX
