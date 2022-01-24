#pragma once

namespace lemons::dsp::synth
{
template <typename SampleType>
class AutomatedHarmonyVoice
{
public:

	using Voice = SynthVoiceBase<SampleType>;

	AutomatedHarmonyVoice (SynthBase<SampleType>& synthToUse, bool shiftUp);

	void setParams (bool shouldBeOn, int newThresh, int newInterval);

	void setEnabled (bool shouldBeEnabled);
	void setThreshold (int newThresh);
	void setInterval (int newInterval);

	void turnNoteOffIfOn();

	bool isAutomatedPitch (int midiNote) { return isOn && lastPitch == midiNote; }

	Voice* getVoice();

private:

	friend class SynthBase<SampleType>;

	void apply();
	void setNoteToOff() { lastPitch = -1; }

	// call this function when processing an automated note-off and the voice's keyboard key is still being held
	void autoNoteOffKeyboardKeyHeld (int midiNote);

	const bool shiftingUp;

	bool isOn { false };
	int	 lastPitch { -1 };
	int	 thresh { 0 };
	int	 interval { 12 };

	SynthBase<SampleType>& synth;
};

}  // namespace lemons::dsp::synth
