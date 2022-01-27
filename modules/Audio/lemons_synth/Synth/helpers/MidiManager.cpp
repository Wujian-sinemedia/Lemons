
namespace lemons::dsp::synth
{
template <typename SampleType>
void MidiManager<SampleType>::handleMidiMessage (const MidiMessage& m)
{
	router.process (m);
}

template <typename SampleType>
void MidiManager<SampleType>::renderChunk (AudioBuffer<SampleType>& audio, MidiBuffer&)
{
	const auto numSamples = audio.getNumSamples();

	for (auto* voice : synth.voices)
	{
		if (voice->isVoiceActive())
			voice->renderBlock (audio);
		else
			voice->bypassedBlock (numSamples);
	}
}

/*---------------------------------------------------------------------------------------------------*/

template <typename SampleType>
void MidiManager<SampleType>::MidiRouter::handleNoteOn (int midiPitch, float velocity)
{
	synth.noteOn (midiPitch, velocity, true, getLastMidiChannel());
}

template <typename SampleType>
void MidiManager<SampleType>::MidiRouter::handleNoteOff (int midiPitch, float velocity)
{
	synth.noteOff (midiPitch, velocity, true, getLastMidiChannel());
}

template <typename SampleType>
void MidiManager<SampleType>::MidiRouter::handleAftertouch (int noteNumber, int aftertouchValue)
{
	jassert (noteNumber >= 0 && noteNumber <= 127);
	jassert (aftertouchValue >= 0 && aftertouchValue <= 127);

	synth.aggregateMidiBuffer.addEvent (MidiMessage::aftertouchChange (getLastMidiChannel(),
																	   noteNumber, aftertouchValue),
										getLastMidiTimestamp());

	for (auto* voice : synth.voices)
		if (voice->isVoiceActive() && voice->getCurrentlyPlayingNote() == noteNumber)
			voice->aftertouchChanged (aftertouchValue);
}

template <typename SampleType>
void MidiManager<SampleType>::MidiRouter::handleChannelPressure (int channelPressureValue)
{
	jassert (channelPressureValue >= 0 && channelPressureValue <= 127);

	synth.aggregateMidiBuffer.addEvent (MidiMessage::channelPressureChange (getLastMidiChannel(),
																			channelPressureValue),
										getLastMidiTimestamp());

	for (auto* voice : synth.voices)
		voice->aftertouchChanged (channelPressureValue);
}

template <typename SampleType>
void MidiManager<SampleType>::MidiRouter::handleSustainPedal (int controllerValue)
{
	const bool isDown = (controllerValue >= 64);

	synth.aggregateMidiBuffer.addEvent (MidiMessage::controllerEvent (getLastMidiChannel(), 0x40, controllerValue),
										getLastMidiTimestamp());

	if (! isDown && ! synth.latchIsOn)
		synth.turnOffAllKeyupNotes (false, false, 0.0f, false);
}

template <typename SampleType>
void MidiManager<SampleType>::MidiRouter::handleSostenutoPedal (int controllerValue)
{
	const bool isDown = (controllerValue >= 64);

	synth.aggregateMidiBuffer.addEvent (MidiMessage::controllerEvent (getLastMidiChannel(),
																	  0x42, controllerValue),
										getLastMidiTimestamp());

	if (isDown && ! synth.latchIsOn)
	{
		for (auto* voice : synth.voices)
			if (voice->isVoiceActive() && ! voice->isPedalPitchVoice && ! voice->isDescantVoice)
				voice->sustainingFromSostenutoPedal = true;
	}
	else
	{
		synth.turnOffAllKeyupNotes (false, false, 0.0f, true);
	}
}

template <typename SampleType>
void MidiManager<SampleType>::MidiRouter::handleSoftPedal (int controllerValue)
{
	const bool isDown = controllerValue >= 64;

	synth.aggregateMidiBuffer.addEvent (MidiMessage::controllerEvent (getLastMidiChannel(),
																	  0x43, controllerValue),
										getLastMidiTimestamp());

	for (auto* voice : synth.voices)
		voice->softPedalChanged (isDown);
}

template <typename SampleType>
void MidiManager<SampleType>::MidiRouter::handlePitchwheel (int wheelValue)
{
	jassert (wheelValue >= 0 && wheelValue <= 127);

	synth.aggregateMidiBuffer.addEvent (MidiMessage::pitchWheel (getLastMidiChannel(),
																 wheelValue),
										getLastMidiTimestamp());

	synth.pitch.bend.newPitchbendReceived (wheelValue);

	for (auto* voice : synth.voices)
		if (voice->isVoiceActive())
			voice->setTargetOutputFrequency (synth.pitch.getFrequencyForMidi (voice->getCurrentlyPlayingNote()));
}


template class MidiManager<float>;
template class MidiManager<double>;

}  // namespace lemons::dsp::synth
