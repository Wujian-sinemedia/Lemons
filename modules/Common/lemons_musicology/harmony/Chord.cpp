namespace lemons::music
{

bool Chord::operator== (const Chord& other) const
{
	return midiNotes == other.midiNotes;
}

bool Chord::operator!= (const Chord& other) const
{
	return ! (*this == other);
}

bool Chord::containsSamePitchClassesAs (const Chord& other) const
{
	return true;
}

bool Chord::containsPitch (int midiNote) const
{
	jassert (midiNote >= 0 && midiNote <= 127);
	return midiNotes.contains (midiNote);
}

bool Chord::containsPitchClass (int pitchClass) const
{
	jassert (pitchClass >= 0 && pitchClass <= 11);

	for (auto note : midiNotes)
		if (note % 12 == pitchClass)
			return true;

	return false;
}

int Chord::getNumPitches() const
{
	return midiNotes.size();
}

int Chord::getNumUniquePitchClasses() const
{
	return 0;
}

int Chord::getLowestPitch() const
{
	return juce::findMinimum (midiNotes.getRawDataPointer(), midiNotes.size());
}

int Chord::getHighestPitch() const
{
	return juce::findMaximum (midiNotes.getRawDataPointer(), midiNotes.size());
}

}  // namespace lemons::music
