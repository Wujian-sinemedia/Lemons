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

namespace lemons::music::scales
{

bool Chromatic::operator== (const Chromatic& other) const
{
	return startingPitchClass == other.startingPitchClass;
}

bool Chromatic::operator!= (const Chromatic& other) const
{
	return startingPitchClass != other.startingPitchClass;
}

juce::Array<int> Chromatic::getIntervalsAsSemitones() const
{
	juce::Array<int> intervals;

	for (auto i = 0; i < 12; ++i)
		intervals.add (1);

	return intervals;
}

PitchClass Chromatic::getPitchClassOfRoot() const noexcept
{
	return startingPitchClass;
}

String Chromatic::getStringDescription() const
{
	return startingPitchClass.getAsString() + " " + TRANS ("chromatic");
}

int Chromatic::notesPerOctave() const noexcept
{
	return 12;
}

Chromatic Chromatic::fromStringDescription (const String& string)
{
	const PitchClass pc { string.upToFirstOccurrenceOf (TRANS ("chromatic"), false, true).trim() };

	return Chromatic { pc };
}

}  // namespace lemons::music::scales
