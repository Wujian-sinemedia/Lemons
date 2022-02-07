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

	const auto root = stringToPitchClass (description.upToFirstOccurrenceOf (" ", false, false).trim());

	return KeySignature { type, root };
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

bool KeySignature::hasEnharmonicKey() const noexcept
{
	return rootHasEnharmonicKey (getPitchClassOfRoot());
}

KeySignature KeySignature::getEnharmonicKey() const noexcept
{
	const auto root = getPitchClassOfRoot();

	if (! rootHasEnharmonicKey (root))
		return { *this };

	return KeySignature { type, ! isFlat, root };
}

bool KeySignature::isEnharmonicKeyOf (const KeySignature& other) const noexcept
{
	return getPitchClassOfRoot() == other.getPitchClassOfRoot() && type == other.type && isFlat != other.isFlat;
}

KeySignature KeySignature::getRelativeKey() const noexcept
{
	const auto otherType = [t = type]
	{
		if (t == Type::Major)
			return Type::NaturalMinor;

		return Type::Major;
	}();

	return KeySignature { numAccidentals, isFlat, otherType };
}

KeySignature KeySignature::getParallelKey() const noexcept
{
	const auto otherType = [t = type]
	{
		if (t == Type::Major)
			return Type::NaturalMinor;

		return Type::Major;
	}();

	return KeySignature { otherType, isFlat, getPitchClassOfRoot() };
}

bool KeySignature::isParallelKeyOf (const KeySignature& other) const noexcept
{
	return hasOppositeTonality (other) && isFlat == other.isFlat && getPitchClassOfRoot() == other.getPitchClassOfRoot();
}

KeySignature KeySignature::getDominantKey() const noexcept
{
	const auto dominant = makeValidPitchClass (getPitchClassOfRoot() + 7);

	return KeySignature { Type::Major, dominant };
}

bool KeySignature::isDominantKeyOf (const KeySignature& other) const noexcept
{
	if (type != Type::Major)
		return false;

	return getPitchClassOfRoot() == makeValidPitchClass (other.getPitchClassOfRoot() + 7);
}

int KeySignature::getPitchClassOfScaleDegree (int scaleDegree) const noexcept
{
	scaleDegree %= 8;

	return makeValidPitchClass (getPitchClassOfRoot() + scaleDegree);
}

}  // namespace lemons::music::scales
