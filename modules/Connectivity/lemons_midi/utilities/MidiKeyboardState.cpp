
namespace lemons::midi
{

static inline bool isValidMidiPitch (int note)
{
	return note >= 0 && note <= 127;
}

static inline auto toSizeType (int note)
{
	return static_cast<std::array<bool, 128>::size_type> (note);
}

MidiKeyboardState::MidiKeyboardState()
{
	allNotesOff();
}

bool MidiKeyboardState::isNoteOn (int note) const
{
	if (! isValidMidiPitch (note)) return false;

	return notes[toSizeType (note)];
}

void MidiKeyboardState::noteOn (int note)
{
	if (! isValidMidiPitch (note)) return;

	notes[toSizeType (note)] = true;
}

void MidiKeyboardState::noteOff (int note)
{
	if (! isValidMidiPitch (note)) return;

	notes[toSizeType (note)] = false;
}

void MidiKeyboardState::processMidiMessage (const MidiMessage& m)
{
	if (m.isNoteOn())
		noteOn (m.getNoteNumber());
	else if (m.isNoteOff())
		noteOff (m.getNoteNumber());
	else if (m.isAllNotesOff())
		allNotesOff();
}

void MidiKeyboardState::setFromArrayOfPitches (const juce::Array<int>& pitches)
{
	if (pitches.isEmpty())
	{
		allNotesOff();
		return;
	}

	for (int i = 0; i <= 127; ++i)
		notes[toSizeType (i)] = pitches.contains (i);
}

juce::Array<int> MidiKeyboardState::getAsArrayOfPitches() const
{
	juce::Array<int> pitches;

	for (int i = 0; i <= 127; ++i)
		if (notes[toSizeType (i)])
			pitches.add (i);

	return pitches;
}

void MidiKeyboardState::allNotesOff()
{
	for (int i = 0; i <= 127; ++i)
		notes[toSizeType (i)] = false;
}

}  // namespace lemons::midi
