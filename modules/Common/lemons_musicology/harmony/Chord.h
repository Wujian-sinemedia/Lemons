#pragma once

namespace lemons::music
{

class Chord final
{
public:

	bool operator== (const Chord& other) const;
	bool operator!= (const Chord& other) const;

	[[nodiscard]] bool containsSamePitchClassesAs (const Chord& other) const;

	[[nodiscard]] bool containsPitch (int midiNote) const;

	[[nodiscard]] bool containsPitchClass (int pitchClass) const;

	[[nodiscard]] int getNumPitches() const;

	[[nodiscard]] int getNumUniquePitchClasses() const;

	[[nodiscard]] int getLowestPitch() const;

	[[nodiscard]] int getHighestPitch() const;

	// isMajor, isMinor, isDiminished, isDomSeventh ...

	// funcs to return various manipulated versions of this chord
	// up/down octave (or other interval transpositions)...
	// func to see if chord fits in key signature

	// get root note

	// get pitch classes?

private:
	juce::Array<int> midiNotes;
};

}  // namespace lemons::music
