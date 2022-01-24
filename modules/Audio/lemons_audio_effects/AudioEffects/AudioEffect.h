#pragma once

#include <lemons_dsp/lemons_dsp.h>

namespace lemons::dsp::FX
{
/**
    Base class for any kind of audio effect.
    Note that AudioEffect does not implement SerializableData::serialize(), your subclass must implement this to save and restore its state!
 */
template <typename SampleType>
struct AudioEffect
{
	/** Destructor. */
	virtual ~AudioEffect() = default;

	/** Your subclass must implement this function with its custom processing logic. */
	virtual void process (AudioBuffer<SampleType>& audio) = 0;

	/** Gives the effect a chance to prepare itself. Implementing this is optional. */
	virtual void prepare (double samplerate, int blocksize);

	/** Notifies the effect when a bypassed block is received. Implementing this is optional. */
	virtual void bypassedBlock (int numSamples);
};


/**
    Base class for audio effects that accept a sidechain signal.
 */
template <typename SampleType>
struct SidechainableAudioEffect : AudioEffect<SampleType>
{
	/** Your subclass must implement this function with its custom processing logic. */
	virtual void process (AudioBuffer<SampleType>& inOut, const AudioBuffer<SampleType>& sidechain) = 0;

	/** Processes the effect with the signal sidechained to itself. */
	void process (AudioBuffer<SampleType>& audio) override;
};


/**
    Base class for audio effects that can report their level or gain reduction.
    This is designed so that subclasses process a single channel at a time, report the level or gain reduction for each channel, and then the user can query a level for a certain channel or the average level of all channels.
 */
template <typename SampleType>
struct LevelReportingAudioEffect : SidechainableAudioEffect<SampleType>
{
	/** Processes the effect with a sidechain signal. */
	void process (AudioBuffer<SampleType>& inOut, const AudioBuffer<SampleType>& sidechain) final;

	/** Processes the effect with the signal sidechained to itself. */
	void process (AudioBuffer<SampleType>& audio) final;

	/** Your subclass must implement this function to process a single channel of audio and return the level or gain reduction for this channel.
	    @param channel The index in the original buffer of the channel currently being processed
	    @param numSamples The total number of samples to process.
	    @param signal Pointer to contiguous audio samples, used for in and out.
	    @param sidechain Pointer to contiguous sidechain signal samples.

	    @return The level or gain reduction for this frame of audio for this channel.
	 */
	virtual SampleType processChannel (int               channel,
	                                   int               numSamples,
	                                   SampleType*       signal,
	                                   const SampleType* sidechain) = 0;

	/** Returns a gain reduction value for a certain channel.
	    This is identical to getLevel().
	 */
	SampleType getGainReduction (int channel) const;

	/** Returns the average gain reduction for all channels.
	    This is identical to getAverageLevel().
	 */
	SampleType getAverageGainReduction() const;

	/** Returns a level value for a certain channel.
	    This is identical to getGainReduction().
	 */
	SampleType getLevel (int channel) const;

	/** Returns the average level for all channels.
	    This is identical to getAverageGainReduction().
	 */
	SampleType getAverageLevel() const;

private:
	juce::Array<SampleType> gainReductions;
};

}  // namespace lemons::dsp::FX
