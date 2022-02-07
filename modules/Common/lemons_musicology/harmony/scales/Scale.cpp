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

bool Scale::operator== (const Scale& other) const
{
	return getPitchClassOfRoot() == other.getPitchClassOfRoot() && getIntervalsAsSemitones() == other.getIntervalsAsSemitones();
}

bool Scale::operator!= (const Scale& other) const
{
	return ! (*this == other);
}

juce::Array<Interval> Scale::getIntervals() const
{
	juce::Array<Interval> intervals;

	for (const auto interval : getIntervalsAsSemitones())
		intervals.add (Interval::fromNumSemitones (interval));

	return intervals;
}

bool Scale::containsPitch (const Pitch& pitch) const
{
	return containsPitchClass (pitch.getPitchClass());
}

bool Scale::containsPitch (int midiNoteNumber) const
{
	return containsPitchClass (midiNoteNumber % 12);
}

bool Scale::containsPitchClass (int pitchClass) const
{
	jassert (pitchClass >= 0 && pitchClass <= 11);

	const auto root = getPitchClassOfRoot();

	for (const auto interval : getIntervalsAsSemitones())
	{
		const auto degreeClass = (root + interval) % 12;

		if (degreeClass == pitchClass)
			return true;
	}

	return false;
}

juce::Array<int> Scale::getPitchClasses() const
{
	juce::Array<int> pitchClasses;

	for (int i = 0; i <= 11; ++i)
		if (containsPitchClass (i))
			pitchClasses.add (i);

	return pitchClasses;
}

juce::Array<Pitch> Scale::getPitches (int octaveNumber) const
{
	const auto startingNote = [this, octaveNumber]
	{
		auto starting = (octaveNumber + 1) * 12;

		while (! containsPitch (starting))
			++starting;

		return starting;
	}();

	juce::Array<Pitch> pitches;

	for (const auto interval : getIntervalsAsSemitones())
		pitches.add (Pitch { startingNote + interval });

	return pitches;
}

juce::Array<Pitch> Scale::getPitches (int lowestMidiNote, int highestMidiNote) const
{
	const auto startingNote = [this, &lowestMidiNote]
	{
		while (! containsPitch (lowestMidiNote))
			++lowestMidiNote;

		return lowestMidiNote;
	}();

	juce::Array<Pitch> pitches;

	const auto intervals = getIntervalsAsSemitones();

	int idx { 0 }, octaveOffset { 0 };

	auto lastNote = startingNote;

	while (lastNote <= highestMidiNote)
	{
		lastNote = startingNote + octaveOffset + intervals.getUnchecked (idx++);

		pitches.add (Pitch { lastNote });

		if (idx >= intervals.size())
		{
			idx = 0;
			octaveOffset += 8;
		}
	}

	return pitches;
}

Pitch Scale::getRoot (int octaveNumber) const noexcept
{
	const auto octaveStart = (octaveNumber + 1) * 12;

	return Pitch { octaveStart + getPitchClassOfRoot() };
}

String Scale::getRootAsString() const noexcept
{
	return pitchClassToString (getPitchClassOfRoot(), getNumSharps() > getNumFlats());
}

}  // namespace lemons::music
