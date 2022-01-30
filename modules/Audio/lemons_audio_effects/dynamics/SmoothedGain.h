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


namespace lemons::dsp::FX
{
/**
	A handy effect class that allows you to change the gain of a stream of audio, and will smooth the changes.
	Internally, this uses my ValueSmoother class.
	@see ValueSmoother, AudioEffect
 */
template <typename SampleType, size_t channels = 1>
class SmoothedGain : public AudioEffect<SampleType>
{
public:

	/** Creates a default SmoothedGain with an initial gain of 1. */
	SmoothedGain();

	/** Creates a SmoothedGain with a specified initial gain value. */
	explicit SmoothedGain (float gain);

	/** Prepares the SmoothedGain. */
	void prepare (double samplerate, int blocksize) final;

	/** Processes the next frame of audio. */
	void process (AudioBuffer<SampleType>& audio) final;

	/** Call this to inform the SmoothedGain that a bypassed block was received.
		This will skip ahead in the ramp created by the smoother.
	*/
	void bypassedBlock (int numSamples) final;

	/** Sets the gain value.
		The next time process() is called, the frame of audio will be ramped from the previous gain value to this new gain value.
	*/
	void setGain (float gain);

	/** Returns the current gain value. */
	float getGain() const;

	/** Resets the SmoothedGain's internal state to a neutral one. */
	void reset();

private:

	juce::OwnedArray<ValueSmoother<SampleType>> smoothers;

	int	  lastBlocksize { 0 };
	float gainVal { 1.f };
};

}  // namespace lemons::dsp::FX
