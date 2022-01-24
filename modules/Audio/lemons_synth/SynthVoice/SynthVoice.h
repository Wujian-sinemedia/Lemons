
#pragma once

#include "TimbreMod.h"

namespace lemons::dsp
{
template <typename SampleType>
class SynthVoiceBase
{
	using uint32	 = juce::uint32;
	using ADSR		 = juce::ADSR;
	using ADSRParams = juce::ADSR::Parameters;

public:
	explicit SynthVoiceBase (SynthBase<SampleType>* base, double initSamplerate = 44100.0);

	virtual ~SynthVoiceBase() = default;

	void prepare (double samplerate, int blocksize);

	void renderBlock (AudioBuffer<SampleType>& output);

	void bypassedBlock (int numSamples);

	void setTargetOutputFrequency (float newFreq);

	/** @name synth_voice_attributes Attribute queries */
	///@{

	[[nodiscard]] bool isCurrentPedalVoice() const noexcept { return isPedalPitchVoice; }
	[[nodiscard]] bool isCurrentDescantVoice() const noexcept { return isDescantVoice; }

	[[nodiscard]] int getCurrentMidiPan() const noexcept { return panner.getLastMidiPan(); }

	[[nodiscard]] bool isKeyDown() const noexcept { return keyIsDown; }

	[[nodiscard]] bool wasStartedBefore (const SynthVoiceBase& other) const noexcept { return noteOnTime < other.noteOnTime; }

	[[nodiscard]] bool isPlayingButReleased() const noexcept { return playingButReleased; }

	[[nodiscard]] bool isVoiceActive() const noexcept { return currentlyPlayingNote >= 0; }

	[[nodiscard]] int getCurrentlyPlayingNote() const noexcept { return currentlyPlayingNote; }

	[[nodiscard]] int getMidiChannel() const { return midiChannel > 0 ? midiChannel : parent->midi.router.getLastMidiChannel(); }

	///@}

	/** @name synth_voice_pitch_glide Pitch glide */
	///@{

	void setPitchGlideTime (double glideTimeSeconds);
	void togglePitchGlide (bool shouldGlide);

	///@}

protected:
	friend class SynthBase<SampleType>;
	friend class synth::AutomatedHarmonyVoice<SampleType>;
	friend class synth::PanningManager<SampleType>;
	friend class synth::MidiManager<SampleType>;

	/*
			Called in the subclass to actually generate some audio at the desired frequency.
			The output buffer sent to this function will contain the number of samples desired for this frame, and your output samples should start at index 0.
		*/
	virtual void renderPlease (AudioBuffer<SampleType>& output, float desiredFrequency, double currentSamplerate) = 0;

	virtual void prepared (double /*samplerate*/, int /*blocksize*/) { }

	virtual void released() { }

	virtual void noteCleared() { }

	virtual void bypassedBlockReceived (float /*voicesLastOutputFreq*/, double /*currentSamplerate*/, int /*numSamples*/) { }

	/* called each time the parent receives a new block in renderVoices(), before any calls to renderPlease() are made. This function may be called even if the voice is not currently active. */
	virtual void newBlockComing (int /*previousBlocksize*/, int /*upcomingBlocksize*/) { }

	/*=================================================================================
		 =================================================================================*/

private:
	void renderInternal (int totalNumSamples);

	void startNote (const int	 midiPitch,
					const float	 velocity,
					const uint32 noteOnTimestamp,
					const bool	 keyboardKeyIsDown = true,
					const bool	 isPedal		   = false,
					const bool	 isDescant		   = false,
					const int	 midichannel	   = -1);

	void stopNote (const float velocity, const bool allowTailOff);

	void clearCurrentNote();

	void setKeyDown (bool isNowDown);

	void setPan (int newPan);

	void setVelocityMultiplier (const float newMultiplier);

	void softPedalChanged (bool isDown);

	void aftertouchChanged (const int newAftertouchValue);

	void setAdsrParameters (const ADSRParams newParams) { adsr.setParameters (newParams); }
	void setQuickReleaseParameters (const ADSRParams newParams) { quickRelease.setParameters (newParams); }

	[[nodiscard]] bool isVoiceOnRightNow() const;

	void resetRampedValues();


	SynthBase<SampleType>* parent;

	ADSR adsr, quickRelease;

	bool keyIsDown { false }, playingButReleased { false }, sustainingFromSostenutoPedal { false }, isQuickFading { false }, pitchGlide { false };

	bool isPedalPitchVoice { false }, isDescantVoice { false }, isDoubledByAutomatedVoice { false };

	int	   currentlyPlayingNote { -1 }, currentAftertouch { 0 }, midiChannel { 1 };
	float  lastReceivedVelocity { 0 };
	double pitchGlideTimeSecs { 0.4 };

	uint32 noteOnTime { 0 };

	ValueSmoother<SampleType> outputFrequency;

	FX::MonoToStereoPanner<SampleType> panner;

	FX::SmoothedGain<SampleType, 1> midiVelocityGain, aftertouchGain;

	AudioBuffer<SampleType> scratchBuffer, renderingBuffer, stereoBuffer;

	synth::TimbreMod<SampleType> playingButReleasedMod { parent->playingButReleased };
	synth::TimbreMod<SampleType> softPedalMod { parent->softPedal };

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthVoiceBase)
};


}  // namespace lemons::dsp
