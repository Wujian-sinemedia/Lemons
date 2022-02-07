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

juce::Array<int> Mode::getIntervalsAsSemitones() const
{
	switch (type)
	{
		case (Type::Ionian) : return { 2, 2, 1, 2, 2, 2, 1 };
		case (Type::Dorian) : return { 2, 1, 2, 2, 2, 1, 2 };
		case (Type::Phrygian) : return { 1, 2, 2, 2, 1, 2, 2 };
		case (Type::Lydian) : return { 2, 2, 2, 1, 2, 2, 1 };
		case (Type::Mixolydian) : return { 2, 2, 1, 2, 2, 1, 2 };
		case (Type::Aeolian) : return { 2, 1, 2, 2, 1, 2, 2 };
		case (Type::Locrian) : return { 1, 2, 2, 1, 2, 2, 2 };
	}

	jassertfalse;
	return {};
}

int Mode::getPitchClassOfRoot() const noexcept
{
	return rootPitchClass;
}

String Mode::getStringDescription() const
{
	const auto typeString = [t = type]
	{
		switch (t)
		{
			case (Type::Ionian) : return TRANS ("ionian");
			case (Type::Dorian) : return TRANS ("dorian");
			case (Type::Phrygian) : return TRANS ("phrygian");
			case (Type::Lydian) : return TRANS ("lydian");
			case (Type::Mixolydian) : return TRANS ("mixolydian");
			case (Type::Aeolian) : return TRANS ("aeolian");
			case (Type::Locrian) : return TRANS ("locrian");
		}

		jassertfalse;
		return String {};
	}();

	return pitchClassToString (rootPitchClass) + " " + typeString;
}

Mode Mode::fromStringDescription (const String& description)
{
	const auto type = [typeString = description.fromFirstOccurrenceOf (" ", false, false).trim()]
	{
		if (typeString.equalsIgnoreCase (TRANS ("ionian")))
			return Type::Ionian;

		if (typeString.equalsIgnoreCase (TRANS ("dorian")))
			return Type::Dorian;

		if (typeString.equalsIgnoreCase (TRANS ("phrygian")))
			return Type::Phrygian;

		if (typeString.equalsIgnoreCase (TRANS ("lydian")))
			return Type::Lydian;

		if (typeString.equalsIgnoreCase (TRANS ("mixolydian")))
			return Type::Mixolydian;

		if (typeString.equalsIgnoreCase (TRANS ("aeolian")))
			return Type::Aeolian;

		if (typeString.equalsIgnoreCase (TRANS ("locrian")))
			return Type::Locrian;

		jassertfalse;
		return Type::Ionian;
	}();

	const auto root = stringToPitchClass (description.upToFirstOccurrenceOf (" ", false, false).trim());

	return Mode { type, root };
}

int Mode::notesPerOctave() const noexcept
{
	return 8;
}

}  // namespace lemons::music::scales
