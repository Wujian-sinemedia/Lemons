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

int Mode::getNumSharps() const noexcept
{
	const auto addedSharps = [t = type]
	{
		if (t == Type::Lydian)
			return 1;

		return 0;
	}();

	const auto parallelMajor = KeySignature { KeySignature::Type::Major, rootPitchClass };

	return parallelMajor.getNumSharps() + addedSharps;
}

int Mode::getNumFlats() const noexcept
{
	const auto addedFlats = [t = type]
	{
		switch (t)
		{
			case (Type::Ionian) : return 0;
			case (Type::Dorian) : return 2;
			case (Type::Phrygian) : return 4;
			case (Type::Lydian) : return 0;
			case (Type::Mixolydian) : return 1;
			case (Type::Aeolian) : return 3;
			case (Type::Locrian) : return 5;
		}

		jassertfalse;
		return 0;
	}();

	const auto parallelMajor = KeySignature { KeySignature::Type::Major, rootPitchClass };

	return parallelMajor.getNumFlats() + addedFlats;
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
			case (Type::Ionian) : return "ionian";
			case (Type::Dorian) : return "dorian";
			case (Type::Phrygian) : return "phrygian";
			case (Type::Lydian) : return "lydian";
			case (Type::Mixolydian) : return "mixolydian";
			case (Type::Aeolian) : return "aeolian";
			case (Type::Locrian) : return "locrian";
		}

		jassertfalse;
		return "";
	}();

	return pitchClassToString (rootPitchClass) + " " + typeString;
}

Mode Mode::fromStringDescription (const String& description)
{
	const auto type = [typeString = description.fromFirstOccurrenceOf (" ", false, false).trim()]
	{
		if (typeString.equalsIgnoreCase ("ionian"))
			return Type::Ionian;

		if (typeString.equalsIgnoreCase ("dorian"))
			return Type::Dorian;

		if (typeString.equalsIgnoreCase ("phrygian"))
			return Type::Phrygian;

		if (typeString.equalsIgnoreCase ("lydian"))
			return Type::Lydian;

		if (typeString.equalsIgnoreCase ("mixolydian"))
			return Type::Mixolydian;

		if (typeString.equalsIgnoreCase ("aeolian"))
			return Type::Aeolian;

		if (typeString.equalsIgnoreCase ("locrian"))
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
