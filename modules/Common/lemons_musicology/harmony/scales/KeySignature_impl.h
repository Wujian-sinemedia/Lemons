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

#pragma once

namespace lemons::music
{

constexpr KeySignature::KeySignature (Type typeToUse, bool isSharps, int pitchClassOfRoot) noexcept
	: type (typeToUse), isFlat (! isSharps)
{
	pitchClassOfRoot %= 12;

	struct MajorMinorPair final
	{
		int numMajorAccidentals, numMinorAccidentals;
	};

	const auto pair = [root = pitchClassOfRoot, flat = isFlat]() -> MajorMinorPair
	{
		if (flat)
		{
			switch (root)
			{
				case (0) : return { 0, 3 };
				case (1) : return { 6, -1 };
				case (2) : return { -1, 1 };
				case (3) : return { 3, 6 };
				case (4) : jassertfalse; return { -1, -1 };
				case (5) : return { 1, 4 };
				case (6) : return { 6, -1 };
				case (7) : return { -1, 2 };
				case (8) : return { 4, 7 };
				case (9) : return { -1, 0 };
				case (10) : return { 2, 5 };
				case (11) : return { 7, -1 };
				default : jassertfalse; return { -1, -1 };
			}
		}

		switch (root)
		{
			case (0) : return { 0, -1 };
			case (1) : return { 7, 4 };
			case (2) : return { 2, -1 };
			case (3) : return { -1, 6 };
			case (4) : return { 4, 1 };
			case (5) : jassertfalse; return { -1, -1 };
			case (6) : return { 6, 3 };
			case (7) : return { 1, -1 };
			case (8) : return { -1, 5 };
			case (9) : return { 3, 0 };
			case (10) : return { -1, 7 };
			case (11) : return { 5, 2 };
			default : jassertfalse; return { -1, -1 };
		}
	}();

	if (type == Type::Major)
		numAccidentals = pair.numMajorAccidentals;
	else
		numAccidentals = pair.numMinorAccidentals;

	jassert (numAccidentals >= 0 && numAccidentals <= 7);
}

constexpr KeySignature::KeySignature (Type typeToUse, int pitchClassOfRoot) noexcept
	: KeySignature (typeToUse, useSharpsForRootByDefault (pitchClassOfRoot), pitchClassOfRoot)
{
}

constexpr bool KeySignature::useSharpsForRootByDefault (int root) noexcept
{
	return root == 2 || root == 4 || root == 7 || root == 9 || root == 11;
}

constexpr KeySignature::KeySignature() noexcept
	: KeySignature (0, false, Type::Major)
{
}

constexpr KeySignature::KeySignature (int numSharpsOrFlats, bool isSharps, Type typeToUse) noexcept
	: numAccidentals (numSharpsOrFlats), isFlat (! isSharps), type (typeToUse)
{
	jassert (numAccidentals >= 0 && numAccidentals <= 7);
}

constexpr KeySignature::KeySignature (const KeySignature& other) noexcept
	: numAccidentals (other.numAccidentals), isFlat (other.isFlat), type (other.type)
{
}

constexpr KeySignature KeySignature::getRelativeKey() const noexcept
{
	const auto otherType = [t = type]
	{
		if (t == Type::Major)
			return Type::NaturalMinor;

		return Type::Major;
	}();

	return KeySignature { numAccidentals, isFlat, otherType };
}

constexpr bool KeySignature::isRelativeKeyOf (const KeySignature& other) const noexcept
{
	const auto oppositeTonality = [t1 = type, t2 = other.type]
	{
		if (t1 == Type::Major)
			return t2 != Type::Major;

		return t2 == Type::Major;
	}();

	return oppositeTonality && numAccidentals == other.numAccidentals && isFlat == other.isFlat;
}

constexpr KeySignature KeySignature::getParallelKey() const noexcept
{
	const auto otherType = [t = type]
	{
		if (t == Type::Major)
			return Type::NaturalMinor;

		return Type::Major;
	}();

	return KeySignature { otherType, isFlat, getPitchClassOfRoot() };
}

constexpr bool KeySignature::isParallelKeyOf (const KeySignature& other) const noexcept
{
	const auto oppositeTonality = [t1 = type, t2 = other.type]
	{
		if (t1 == Type::Major)
			return t2 != Type::Major;

		return t2 == Type::Major;
	}();

	return oppositeTonality && isFlat == other.isFlat && getPitchClassOfRoot() == other.getPitchClassOfRoot();
}

constexpr bool KeySignature::hasEnharmonicKey() const noexcept
{
	return rootHasEnharmonicKey (getPitchClassOfRoot());
}

constexpr bool KeySignature::rootHasEnharmonicKey (int root) noexcept
{
	return root == 11 || root == 8 || root == 6 || root == 3 || root == 1;
}

constexpr KeySignature KeySignature::getEnharmonicKey() const noexcept
{
	const auto root = getPitchClassOfRoot();

	if (! rootHasEnharmonicKey (root))
		return { *this };

	return KeySignature { type, ! isFlat, root };
}

constexpr bool KeySignature::isEnharmonicKeyOf (const KeySignature& other) const noexcept
{
	return getPitchClassOfRoot() == other.getPitchClassOfRoot() && type == other.type && isFlat != other.isFlat;
}

constexpr KeySignature KeySignature::getDominantKey() const noexcept
{
	const auto dominant = (getPitchClassOfRoot() + 7) % 12;

	return KeySignature { Type::Major, ! isFlat, dominant };
}

constexpr bool KeySignature::isDominantKeyOf (const KeySignature& other) const noexcept
{
	if (type != Type::Major)
		return false;

	if (isFlat != other.isFlat)
		return false;

	const auto expectedRoot = (other.getPitchClassOfRoot() + 7) % 12;

	return getPitchClassOfRoot() == expectedRoot;
}

constexpr bool KeySignature::operator== (const KeySignature& other) const noexcept
{
	return numAccidentals == other.numAccidentals && isFlat == other.isFlat && type == other.type;
}

constexpr bool KeySignature::operator!= (const KeySignature& other) const noexcept
{
	return ! (*this == other);
}

constexpr bool KeySignature::isFlatKey() const noexcept
{
	return isFlat;
}

constexpr bool KeySignature::isSharpKey() const noexcept
{
	return ! isFlat;
}

constexpr bool KeySignature::isMajorKey() const noexcept
{
	return type == Type::Major;
}

constexpr bool KeySignature::isMinorKey() const noexcept
{
	return ! isMajorKey();
}

constexpr int KeySignature::getPitchClassOfScaleDegree (int scaleDegree) const noexcept
{
	scaleDegree %= 8;

	jassert (scaleDegree >= 0 && scaleDegree <= 8);

	const auto pitchClass = getPitchClassOfRoot() + scaleDegree;

	return pitchClass % 12;
}

}  // namespace lemons::music
