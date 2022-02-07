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
	return ! (*this == other);
}

juce::Array<int> Chromatic::getIntervalsAsSemitones() const
{
	juce::Array<int> intervals;

	for (auto i = 0; i < 12; ++i)
		intervals.add (1);

	return intervals;
}

int Chromatic::getNumSharps() const noexcept
{
	return 7;
}

int Chromatic::getNumFlats() const noexcept
{
	return 7;
}

int Chromatic::getPitchClassOfRoot() const noexcept
{
	return startingPitchClass;
}

String Chromatic::getStringDescription() const
{
	return pitchClassToString (startingPitchClass) + " " + TRANS ("chromatic");
}

int Chromatic::notesPerOctave() const noexcept
{
	return 12;
}

Chromatic Chromatic::fromStringDescription (const String& string)
{
	const auto rootString = string.upToFirstOccurrenceOf (TRANS ("chromatic"), false, true).trim();

	return Chromatic { stringToPitchClass (rootString) };
}

}  // namespace lemons::music::scales
