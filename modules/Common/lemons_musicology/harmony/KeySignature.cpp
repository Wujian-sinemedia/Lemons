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

KeySignature KeySignature::fromStringDescription (const String& description)
{
	const auto isMajor = ! description.containsIgnoreCase ("minor");

	const auto rootDesc = description.upToFirstOccurrenceOf (" ", false, false).trim();

	const auto root = stringToPitchClass (rootDesc);

	if (rootDesc.endsWithChar (getSharpSymbol()) || rootDesc.endsWithChar ('#'))
		return KeySignature { isMajor, true, root };

	if (rootDesc.endsWithChar (getFlatSymbol()) || (rootDesc.endsWithChar ('b') && rootDesc.length() > 1))
		return KeySignature { isMajor, false, root };

	const auto use_sharps = root == 2 || root == 4 || root == 7 || root == 9 || root == 11;

	return KeySignature { isMajor, use_sharps, root };
}

String KeySignature::getRootAsString() const noexcept
{
	return pitchClassToString (getPitchClassOfRoot(), ! isFlat);
}

String KeySignature::getStringDescription() const noexcept
{
	const auto res = getRootAsString();

	if (isMajor)
		return res + " major";

	return res + " minor";
}

String KeySignature::getScaleDegreeAsString (int scaleDegree) const noexcept
{
	return pitchClassToString (getPitchClassOfScaleDegree (scaleDegree), ! isFlat);
}

bool KeySignature::containsPitchClass (int pitchClass) const
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

bool KeySignature::containsPitch (const Pitch& pitch) const
{
	return containsPitchClass (pitch.getPitchClass());
}

bool KeySignature::containsPitch (int midiNoteNumber) const
{
	return containsPitchClass (midiNoteNumber % 11);
}

juce::Array<int> KeySignature::getIntervalsAsSemitones() const
{
	if (isMajor)
		return { 2, 2, 1, 2, 2, 2, 1 };

	// natural minor. TO DO: add support for harmonic & melodic minor....
	return { 2, 1, 2, 2, 1, 2, 2 };
}

juce::Array<Interval> KeySignature::getIntervals() const
{
	juce::Array<Interval> intervals;

	for (const auto interval : getIntervalsAsSemitones())
		intervals.add (Interval::fromNumSemitones (interval));

	return intervals;
}

juce::Array<Pitch> KeySignature::getPitches (int octaveNumber) const
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

juce::Array<Pitch> KeySignature::getPitches (int lowestMidiNote, int highestMidiNote) const
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
			++octaveOffset;
		}
	}

	return pitches;
}

}  // namespace lemons::music
