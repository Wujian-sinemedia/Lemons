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

bool Octatonic::operator== (const Octatonic& other) const
{
	return startWithHalfStep == other.startWithHalfStep && pitchClassOfRoot == other.pitchClassOfRoot;
}

bool Octatonic::operator!= (const Octatonic& other) const
{
	return ! (*this == other);
}

juce::Array<int> Octatonic::getIntervalsAsSemitones() const
{
	juce::Array<int> intervals;

	auto semitone = startWithHalfStep;

	for (auto i = 0; i < 8; ++i)
	{
		if (semitone)
			intervals.add (1);
		else
			intervals.add (2);

		semitone = ! semitone;
	}

	return intervals;
}

PitchClass Octatonic::getPitchClassOfRoot() const noexcept
{
	return pitchClassOfRoot;
}

String Octatonic::getStringDescription() const
{
	const auto kindString = [halfWhole = startWithHalfStep]
	{
		if (halfWhole)
			return TRANS ("half/whole");

		return TRANS ("whole/half");
	}();

	return pitchClassOfRoot.getAsString() + " " + kindString + " " + TRANS ("octatonic");
}

int Octatonic::notesPerOctave() const noexcept
{
	return 8;
}

Octatonic Octatonic::fromStringDescription (const String& string)
{
	const auto halfWhole = string.containsIgnoreCase (TRANS ("half/whole"));

	const PitchClass pc { string.upToFirstOccurrenceOf (" ", false, false).trim() };

	return Octatonic { pc, halfWhole };
}

}  // namespace lemons::music::scales
