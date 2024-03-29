
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

namespace lemons::dsp::synth
{
template <typename SampleType>
AutomatedHarmonyVoice<SampleType>::AutomatedHarmonyVoice (SynthBase<SampleType>& synthToUse, bool shiftUp)
	: shiftingUp (shiftUp), synth (synthToUse)
{
}

static inline bool compareNotes (bool shiftingUp, int a, int b)
{
	if (shiftingUp)
		return a > b;

	return a < b;
}

template <typename SampleType>
void AutomatedHarmonyVoice<SampleType>::apply()
{
	if (! isOn)
	{
		turnNoteOffIfOn();
		return;
	}

	int	   currentExtreme = 128;
	Voice* extremeVoice	  = nullptr;

	// find the current lowest/highest note being played by a keyboard key
	for (auto* voice : synth.voices)
	{
		if (voice->isVoiceActive() && voice->isKeyDown())
		{
			const int note = voice->getCurrentlyPlayingNote();

			if (compareNotes (shiftingUp, note, currentExtreme))
			{
				currentExtreme = note;
				extremeVoice   = voice;
			}
		}
	}

	if (! compareNotes (shiftingUp, currentExtreme, thresh))
	{
		turnNoteOffIfOn();
		return;
	}

	const auto newPitch = shiftingUp ? currentExtreme + interval : currentExtreme - interval;

	// output note hasn't changed, or an impossible midinote, or the new desired pedal pitch is already on
	if (newPitch == lastPitch || newPitch < 0 || newPitch > 127 || synth.isPitchActive (newPitch, false, true))
		return;

	auto* prevVoice = synth.getVoicePlayingNote (lastPitch);  // attempt to keep the pedal line consistent - using the same synth voice

	if (prevVoice != nullptr)
		if (prevVoice->isKeyDown())	 // can't "steal" the voice playing the last pedal note if its keyboard key is down
			prevVoice = nullptr;

	const auto velocity = (extremeVoice != nullptr) ? extremeVoice->lastReceivedVelocity
													: (prevVoice != nullptr ? prevVoice->lastReceivedVelocity : 1.0f);

	if (prevVoice != nullptr)
	{
		//  there was a previously active pedal voice, so steal it directly without calling noteOn:
		synth.startVoice (prevVoice, newPitch, velocity, false, prevVoice->getMidiChannel());
	}
	else
	{
		turnNoteOffIfOn();
		synth.noteOn (newPitch, velocity, false, synth.midi.router.getLastMidiChannel());
	}

	lastPitch = newPitch;

	if (extremeVoice != nullptr)
		extremeVoice->isDoubledByAutomatedVoice = true;
}

template <typename SampleType>
void AutomatedHarmonyVoice<SampleType>::setEnabled (bool shouldBeEnabled)
{
	if (isOn == shouldBeEnabled) return;

	isOn = shouldBeEnabled;

	if (isOn)
		apply();
	else
		turnNoteOffIfOn();
}

template <typename SampleType>
void AutomatedHarmonyVoice<SampleType>::setThreshold (int newThresh)
{
	if (thresh == newThresh) return;

	thresh = newThresh;

	if (isOn) apply();
}

template <typename SampleType>
void AutomatedHarmonyVoice<SampleType>::setInterval (int newInterval)
{
	if (interval == newInterval) return;

	interval = newInterval;

	if (isOn) apply();
}

template <typename SampleType>
void AutomatedHarmonyVoice<SampleType>::setParams (bool shouldBeOn, int newThresh, int newInterval)
{
	thresh	 = newThresh;
	interval = newInterval;

	setEnabled (shouldBeOn);
}

template <typename SampleType>
void AutomatedHarmonyVoice<SampleType>::turnNoteOffIfOn()
{
	if (lastPitch > -1)
	{
		synth.noteOff (lastPitch, 1.0f, false, false);
		lastPitch = -1;
	}
}

template <typename SampleType>
void AutomatedHarmonyVoice<SampleType>::AutomatedHarmonyVoice::autoNoteOffKeyboardKeyHeld (int midiNote)
{
	if (isAutomatedPitch (midiNote))
	{
		lastPitch = -1;

		if (auto* voice = synth.getVoicePlayingNote (midiNote))
		{
			voice->isPedalPitchVoice = false;
			voice->setKeyDown (true);  // refresh the voice's own internal tracking of its key state
		}
	}
}

template <typename SampleType>
SynthVoiceBase<SampleType>* AutomatedHarmonyVoice<SampleType>::AutomatedHarmonyVoice::getVoice()
{
	if (! isOn || lastPitch < 0)
		return nullptr;

	return synth.getVoicePlayingNote (lastPitch);
}


template class AutomatedHarmonyVoice<float>;
template class AutomatedHarmonyVoice<double>;

}  // namespace lemons::dsp::synth
