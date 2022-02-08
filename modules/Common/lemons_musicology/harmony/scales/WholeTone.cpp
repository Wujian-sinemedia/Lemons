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

bool WholeTone::operator== (const WholeTone& other) const
{
	return startingPitchClass == other.startingPitchClass;
}

bool WholeTone::operator!= (const WholeTone& other) const
{
	return startingPitchClass != other.startingPitchClass;
}

juce::Array<int> WholeTone::getIntervalsAsSemitones() const
{
	juce::Array<int> intervals;

	for (auto i = 0; i < 6; ++i)
		intervals.add (2);

	return intervals;
}

PitchClass WholeTone::getPitchClassOfRoot() const noexcept
{
	return startingPitchClass;
}

String WholeTone::getStringDescription() const
{
	return startingPitchClass.getAsString() + " " + TRANS ("whole tone");
}

int WholeTone::notesPerOctave() const noexcept
{
	return 6;
}

WholeTone WholeTone::fromStringDescription (const String& string)
{
	const PitchClass pc { string.upToFirstOccurrenceOf (TRANS ("whole tone"), false, true).trim() };

	return WholeTone { pc };
}

}  // namespace lemons::music::scales
