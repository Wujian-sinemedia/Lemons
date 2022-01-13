namespace lemons::dsp::psola
{

template <typename SampleType>
void MidiShifter<SampleType>::setMidiPitch (float newMidiPitch)
{
	jassert (newMidiPitch >= 0.f && newMidiPitch <= 127.f);

	midiPitch = newMidiPitch;

	setPitchFromMidiPitch();
}

template <typename SampleType>
float MidiShifter<SampleType>::getMidiPitch() const noexcept
{
	return midiPitch;
}

template <typename SampleType>
void MidiShifter<SampleType>::onNewBlock()
{
	setPitchFromMidiPitch();
}

template <typename SampleType>
void MidiShifter<SampleType>::setPitchFromMidiPitch()
{
	this->setPitchHz (math::midiToFreq (midiPitch));
}

template <typename SampleType>
void MidiShifter<SampleType>::pitchHzChanged (int newPitchHz)
{
	midiPitch = static_cast<float> (math::freqToMidi (newPitchHz));
}

template class MidiShifter<float>;
template class MidiShifter<double>;

}  // namespace lemons::dsp::psola
