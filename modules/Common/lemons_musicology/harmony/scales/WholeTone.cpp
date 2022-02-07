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
	return ! (*this == other);
}

juce::Array<int> WholeTone::getIntervalsAsSemitones() const
{
	juce::Array<int> intervals;

	for (auto i = 0; i < 6; ++i)
		intervals.add (2);

	return intervals;
}

int WholeTone::getPitchClassOfRoot() const noexcept
{
	return startingPitchClass;
}

String WholeTone::getStringDescription() const
{
	return pitchClassToString (startingPitchClass) + TRANS (" whole tone");
}

int WholeTone::notesPerOctave() const noexcept
{
	return 6;
}

WholeTone WholeTone::fromStringDescription (const String& string)
{
	const auto rootString = string.upToFirstOccurrenceOf (TRANS ("whole tone"), false, true).trim();

	return WholeTone { stringToPitchClass (rootString) };
}

}  // namespace lemons::music::scales
