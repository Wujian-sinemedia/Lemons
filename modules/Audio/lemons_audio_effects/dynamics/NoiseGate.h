
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
 *  ======================================================================================
 */

#pragma once

#include <juce_dsp/juce_dsp.h>

namespace lemons::dsp::FX
{
/**
	A simple noise gate effect class that allows you to optionally sidechain the signal.
	This class is essentially a refactor of the noise gate class from the juce dsp module, with the sidechaining capabilities added.
	@see LevelReportingAudioEffect, AudioEffect
 */
template <typename SampleType>
class NoiseGate : public LevelReportingAudioEffect<SampleType>
{
public:

	/** Creates a noise gate with some default initial settings. */
	NoiseGate();

	/** Creates a noise gate with some initial settings. */
	NoiseGate (float threshDB, float ratioToUse, float attackMs, float releaseMs, bool shouldBeInverted = false);

	/** Prepares the noise gate. */
	void prepare (double samplerate, int blocksize) final;

	/** Sets whether the gate's behavior should be inverted.
		@param gateBehaviorShouldBeInverted When this is true, only the quietest parts of the signal get through -- ie, ONLY the noise. When this is false, the gate functions normally.
	*/
	void setInverted (bool gateBehaviorShouldBeInverted);

	/** Sets the gate's threshold in decibels. */
	void setThreshold (float newThreshold_dB);

	/** Sets the compression ratio for the gated parts of the signal. */
	void setRatio (float newRatio);

	/** Sets the gate's attack time, in milliseconds. */
	void setAttack (float newAttack_ms);

	/** Sets the gate's release time, in milliseconds. */
	void setRelease (float newRelease_ms);

	/** Resets the noise gate's internal state to a neutral one. */
	void reset();


	/** Processes a single channel of audio.
		@param channel The channel number that this input sample is from, in the original buffer.
		@param numSamples The number of samples to process.
		@param signalToGate Pointer to contiguous input sample values.
		@param sidechain Pointer to contiguous sidechain signal values.
		@return The gain reduction amount for this frame of this channel's audio.
	*/
	SampleType processChannel (int				 channel,
							   int				 numSamples,
							   SampleType*		 signalToGate,
							   const SampleType* sidechain) final;


	/** Processes a single sample.
		@param channel The channel number that this input sample is from, in the original buffer.
		@param sampleToGate The value of the input sample.
		@param sidechainValue The value of the sidechain sample.
		@param gainReduction Pointer to where to write the gain reduction value for this sample. This may be null.
		@return The output gated sample.
	*/
	SampleType processSample (int		  channel,
							  SampleType  sampleToGate,
							  SampleType  sidechainValue,
							  SampleType* gainReduction);


private:

	void update();

	juce::dsp::ProcessSpec spec;

	SampleType								threshold, thresholdInverse, currentRatio;
	juce::dsp::BallisticsFilter<SampleType> envelopeFilter, RMSFilter;

	SampleType thresholddB { -100 }, ratio { 10.0 }, attackTime { 1.0 },
		releaseTime { 100.0 };

	bool inverted { false };
};

}  // namespace lemons::dsp::FX
