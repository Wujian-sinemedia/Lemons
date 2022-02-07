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

constexpr KeySignature::KeySignature (bool isMajorKey, bool isSharps, int pitchClassOfRoot) noexcept
	: isMajor (isMajorKey), isFlat (! isSharps)
{
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

	if (isMajor)
		numAccidentals = pair.numMajorAccidentals;
	else
		numAccidentals = pair.numMinorAccidentals;

	jassert (numAccidentals >= 0 && numAccidentals <= 7);
}

constexpr KeySignature::KeySignature() noexcept
	: KeySignature (0, false, true)
{
}

constexpr KeySignature::KeySignature (int numSharpsOrFlats, bool isSharps, bool isMajorKey) noexcept
	: numAccidentals (numSharpsOrFlats), isFlat (! isSharps), isMajor (isMajorKey)
{
	jassert (numAccidentals >= 0 && numAccidentals <= 7);
}

constexpr KeySignature::KeySignature (const KeySignature& other) noexcept
	: numAccidentals (other.numAccidentals), isFlat (other.isFlat), isMajor (other.isMajor)
{
}

constexpr KeySignature KeySignature::getRelativeKey() const noexcept
{
	return KeySignature { numAccidentals, isFlat, ! isMajor };
}

constexpr bool KeySignature::isRelativeKeyOf (const KeySignature& other) const noexcept
{
	return numAccidentals == other.numAccidentals && isFlat == other.isFlat && isMajor != other.isMajor;
}

constexpr KeySignature KeySignature::getParallelKey() const noexcept
{
	return KeySignature { ! isMajor, isFlat, getPitchClassOfRoot() };
}

constexpr bool KeySignature::isParallelKeyOf (const KeySignature& other) const noexcept
{
	return isFlat == other.isFlat && getPitchClassOfRoot() == other.getPitchClassOfRoot() && isMajor != other.isMajor;
}

constexpr bool pitchClassRootHasEnharmonicKey (int root) noexcept
{
	return root == 11 || root == 8 || root == 6 || root == 3 || root == 1;
}

constexpr bool KeySignature::hasEnharmonicKey() const noexcept
{
	return pitchClassRootHasEnharmonicKey (getPitchClassOfRoot());
}

constexpr KeySignature KeySignature::getEnharmonicKey() const noexcept
{
	const auto root = getPitchClassOfRoot();

	if (! pitchClassRootHasEnharmonicKey (root))
		return { *this };

	return KeySignature { isMajor, ! isFlat, root };
}

constexpr bool KeySignature::isEnharmonicKeyOf (const KeySignature& other) const noexcept
{
	return getPitchClassOfRoot() == other.getPitchClassOfRoot() && isMajor == other.isMajor && isFlat != other.isFlat;
}

constexpr KeySignature KeySignature::getDominantKey() const noexcept
{
	const auto dominant = (getPitchClassOfRoot() + 7) % 12;

	return KeySignature { true, ! isFlat, dominant };
}

constexpr bool KeySignature::isDominantKeyOf (const KeySignature& other) const noexcept
{
	if (! isMajor)
		return false;

	if (isFlat != other.isFlat)
		return false;

	const auto expectedRoot = (other.getPitchClassOfRoot() + 7) % 12;

	return getPitchClassOfRoot() == expectedRoot;
}

constexpr bool KeySignature::operator== (const KeySignature& other) const noexcept
{
	return numAccidentals == other.numAccidentals && isFlat == other.isFlat && isMajor == other.isMajor;
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
	return isMajor;
}

constexpr bool KeySignature::isMinorKey() const noexcept
{
	return ! isMajor;
}

constexpr int KeySignature::getNumSharps() const noexcept
{
	if (isFlat)
		return 0;

	return numAccidentals;
}

constexpr int KeySignature::getNumFlats() const noexcept
{
	if (! isFlat)
		return 0;

	return numAccidentals;
}

constexpr int KeySignature::getPitchClassOfRoot() const noexcept
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
				default : jassertfalse; return { 0, 9 };
			}
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
			default : jassertfalse; return { 0, 9 };
		}
	}();

	if (isMajor)
		return pair.majorRoot;

	return pair.minorRoot;
}

constexpr int KeySignature::getPitchClassOfScaleDegree (int scaleDegree) const noexcept
{
	scaleDegree %= 8;

	jassert (scaleDegree >= 0 && scaleDegree <= 11);

	const auto pitchClass = getPitchClassOfRoot() + scaleDegree;

	return pitchClass % 12;
}

}  // namespace lemons::music
