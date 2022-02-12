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

namespace lemons::music
{

Chord::Chord (const std::initializer_list<int>& midiNotes)
{
	for (const auto midiNote : midiNotes)
		pitches.add (Pitch { midiNote });

	alg::removeDuplicates (pitches);
	pitches.sort();
}

Chord::Chord (const std::initializer_list<Pitch>& midiNotes)
	: pitches (midiNotes)
{
	alg::removeDuplicates (pitches);
	pitches.sort();
}

Chord::Chord (const Chord& other)
	: pitches (other.pitches)
{
}

Chord& Chord::operator= (const Chord& other)
{
	pitches = other.pitches;
	return *this;
}

int Chord::getNumPitches() const
{
	return pitches.size();
}

Pitch Chord::getLowestPitch() const
{
	return juce::findMinimum (pitches.getRawDataPointer(), pitches.size());
}

Pitch Chord::getHighestPitch() const
{
	return juce::findMaximum (pitches.getRawDataPointer(), pitches.size());
}

bool Chord::contains (const PitchClass& pitchClass) const noexcept
{
	return alg::contains_if (pitches,
							 [&pitchClass] (const Pitch& p)
							 { return p.getPitchClass() == pitchClass; });
}

bool Chord::contains (const Pitch& pitch) const noexcept
{
	return pitches.contains (pitch);
}

bool Chord::contains (int midiNote) const noexcept
{
	return alg::contains_if (pitches,
							 [note = midiNote] (const Pitch& p)
							 { return p.getRoundedMidiPitch() == note; });
}

bool Chord::fitsInScale (const scales::Scale& scale) const noexcept
{
	for (const auto& pitch : pitches)
		if (! scale.containsPitch (pitch))
			return false;

	return true;
}

juce::Array<Interval> Chord::getIntervals() const
{
	juce::Array<Interval> intervals;

	for (auto i = 1; i < pitches.size(); ++i)
		intervals.add (Interval::fromPitches (pitches.getUnchecked (i - 1), pitches.getUnchecked (i)));

	return intervals;
}

juce::Array<PitchClass> Chord::getPitchClasses() const
{
	juce::Array<PitchClass> pitchClasses;

	for (const auto& pitch : pitches)
		pitchClasses.add (pitch.getPitchClass());

	alg::removeDuplicates (pitchClasses);

	return pitchClasses;
}

int Chord::getNumUniquePitchClasses() const
{
	return getPitchClasses().size();
}

Chord Chord::applyInterval (const Interval& interval, bool above)
{
	Chord newChord { *this };

	for (auto i = 0; i < newChord.pitches.size(); ++i)
		newChord.pitches.getReference (i) = interval.applyToPitch (newChord.pitches.getUnchecked (i), above);

	return newChord;
}

}  // namespace lemons::music
