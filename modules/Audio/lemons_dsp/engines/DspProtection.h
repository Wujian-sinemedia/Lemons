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

/** @ingroup audio_engines
    A DSP engine that, under normal conditions, simply passes the audio through; if any samples are inf or nan, then the output is muted until you manually un-mute it again.
 */
template <typename SampleType>
class Protector final : public Engine<SampleType>
{
public:

	using CallbackFunc = std::function<void (const AudioBuffer<SampleType>&)>;

	/** Constructor.
	    @param hardClip When true, the mute is also triggered by any samples greater than 1.0 or less than -1.0.
	    @param whenMuteTriggered A function that will be called each time the mute is triggered. This function will be called with the input audio that triggered the protective mute. Note that this will be called from the audio thread; however, since this class is mainly intended for debugging and testing purposes, and because you know that if this is called, your audio has already glitched, it's probably OK to do some stuff like logging in here.
	 */
	explicit Protector (bool hardClip = true, CallbackFunc&& whenMuteTriggered = nullptr);

	/** Returns true if the audio stream is currently muted. */
	[[nodiscard]] bool isMuted() const noexcept;

	/** Activates the mute. */
	void forceMute() noexcept;

	/** Turns off the mute. */
	void resetMute() noexcept;

private:
	void renderBlock (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midi, bool isBypassed) final;

	void released() final;
    
    void prepared (int blocksize, double samplerate, int numChannels) final;

	bool mute { false };

	struct InternalPassthruEngine : public Engine<SampleType>
	{
		void renderBlock (const AudioBuffer<SampleType>& input,
		                  AudioBuffer<SampleType>&       output,
		                  MidiBuffer&, bool isBypassed) final;
	};

	InternalPassthruEngine internalEngine;

	const bool isHardClipping;

	CallbackFunc muteCallback;
};

}  // namespace lemons::dsp
