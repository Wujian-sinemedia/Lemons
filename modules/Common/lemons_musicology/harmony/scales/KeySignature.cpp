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
	const auto type = [&description]
	{
		if (! description.containsIgnoreCase ("minor"))
			return Type::Major;

		if (description.containsIgnoreCase ("harmonic"))
			return Type::HarmonicMinor;

		return Type::NaturalMinor;
	}();

	const auto rootDesc = description.upToFirstOccurrenceOf (" ", false, false).trim();

	const auto root = stringToPitchClass (rootDesc);

	if (rootDesc.endsWithChar (getSharpSymbol()) || rootDesc.endsWithChar ('#'))
		return KeySignature { type, true, root };

	if (rootDesc.endsWithChar (getFlatSymbol()) || (rootDesc.endsWithChar ('b') && rootDesc.length() > 1))
		return KeySignature { type, false, root };

	const auto use_sharps = root == 2 || root == 4 || root == 7 || root == 9 || root == 11;

	return KeySignature { type, use_sharps, root };
}

String KeySignature::getRootAsString() const noexcept
{
	return pitchClassToString (getPitchClassOfRoot(), ! isFlat);
}

String KeySignature::getStringDescription() const
{
	const auto res = getRootAsString();

	if (type == Type::Major)
		return res + " major";

	if (type == Type::HarmonicMinor)
		return res + " harmonic minor";

	return res + " minor";
}

String KeySignature::getScaleDegreeAsString (int scaleDegree) const noexcept
{
	return pitchClassToString (getPitchClassOfScaleDegree (scaleDegree), ! isFlat);
}


juce::Array<int> KeySignature::getIntervalsAsSemitones() const
{
	switch (type)
	{
		case (Type::Major) : return { 2, 2, 1, 2, 2, 2, 1 };
		case (Type::NaturalMinor) : return { 2, 1, 2, 2, 1, 2, 2 };
		case (Type::HarmonicMinor) : return { 2, 1, 2, 2, 1, 3, 1 };
	}

	jassertfalse;
	return {};
}

int KeySignature::getNumSharps() const noexcept
{
	if (isFlat)
		return 0;

	return numAccidentals;
}

int KeySignature::getNumFlats() const noexcept
{
	if (! isFlat)
		return 0;

	return numAccidentals;
}

int KeySignature::getPitchClassOfRoot() const noexcept
{
	struct MajorMinorPair final
	{
		int majorRoot, minorRoot;
	};

	const auto pair = [num = numAccidentals, flat = isFlat]() -> MajorMinorPair
	{
		if (flat)
		{
			switch (num)
			{
				case (0) : return { 0, 9 };
				case (1) : return { 5, 2 };
				case (2) : return { 10, 7 };
				case (3) : return { 3, 0 };
				case (4) : return { 8, 5 };
				case (5) : return { 1, 10 };
				case (6) : return { 6, 3 };
				case (7) : return { 11, 8 };
			}

			jassertfalse;
			return { 0, 9 };
		}

		switch (num)
		{
			case (0) : return { 0, 9 };
			case (1) : return { 7, 4 };
			case (2) : return { 2, 11 };
			case (3) : return { 9, 6 };
			case (4) : return { 4, 1 };
			case (5) : return { 11, 8 };
			case (6) : return { 6, 3 };
			case (7) : return { 1, 10 };
		}

		jassertfalse;
		return { 0, 9 };
	}();

	if (type == Type::Major)
		return pair.majorRoot;

	return pair.minorRoot;
}

int KeySignature::notesPerOctave() const noexcept
{
	return 8;
}

}  // namespace lemons::music
