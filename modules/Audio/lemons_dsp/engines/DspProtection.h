#pragma once

namespace lemons::dsp
{

/** A DSP engine that, under normal conditions, simply passes the audio through; if any samples are inf or nan, then the output is muted until you manually un-mute it again.
 */
template <typename SampleType>
class Protector final : public Engine<SampleType>
{
public:

	/** Constructor.
	    @param hardClip When true, the mute is also triggered by any samples greater than 1.0 or less than -1.0.
	    @param whenMuteTriggered A function that will be called each time the mute is triggered. Note that this will be called from the audio thread; however, since this class is mainly intended for debugging and testing purposes, and because you know that if this is called, your audio has already glitched, it's probably OK to do some stuff like logging in here.
	 */
	explicit Protector (
	    bool hardClip = true, std::function<void()> whenMuteTriggered = []() {});

	/** Returns true if the audio stream is currently muted. */
	[[nodiscard]] bool isMuted() const noexcept;

	/** Activates the mute.
	    @param callbackNow When true, the mute callback function will be called immediately.
	 */
	void forceMute (bool callbackNow = false) noexcept;

	/** Turns off the mute. */
	void resetMute() noexcept;

private:
	void renderBlock (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midi, bool isBypassed) final;

	void released() final;

	bool mute { false };

	struct InternalPassthruEngine : public Engine<SampleType>
	{
		void renderBlock (const AudioBuffer<SampleType>& input,
		                  AudioBuffer<SampleType>&       output,
		                  MidiBuffer&, bool isBypassed) final;
	};

	InternalPassthruEngine internalEngine;

	const bool isHardClipping;

	std::function<void()> muteCallback;
};

}  // namespace lemons::dsp
