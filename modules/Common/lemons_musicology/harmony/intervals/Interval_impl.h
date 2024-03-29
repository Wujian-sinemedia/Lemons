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

constexpr bool Interval::isValidQualityForKind (Quality quality, int kind) noexcept
{
	if (kind == 0 || kind == 4 || kind == 5 || kind == 8)
	{
		const auto base = quality != Quality::Major && quality != Quality::Minor;

		if (kind == 0)
			return base && quality != Quality::Diminished;

		return base;
	}

	return quality != Quality::Perfect;
}

constexpr bool Interval::intervalIsPerfectKind() noexcept
{
	return kind == 0 || kind == 4 || kind == 5 || kind == 8;
}

constexpr Interval::Interval (int kindToUse, Quality qualityToUse) noexcept
	: quality (qualityToUse), kind (kindToUse % 9)
{
	jassert (kind != 1);
	jassert (kind >= 0 && kind <= 8);

	jassert (isValidQualityForKind (quality, kind));
}

constexpr Interval::Interval() noexcept
	: Interval (0, Quality::Perfect)
{
}

constexpr Interval Interval::fromNumSemitones (int semitones) noexcept
{
	switch (math::abs (semitones) % 13)
	{
		case (0) : return Interval { 0, Quality::Perfect };
		case (1) : return Interval { 2, Quality::Minor };
		case (2) : return Interval { 2, Quality::Major };
		case (3) : return Interval { 3, Quality::Minor };
		case (4) : return Interval { 3, Quality::Major };
		case (5) : return Interval { 4, Quality::Perfect };
		case (6) : return Interval { 4, Quality::Augmented };
		case (7) : return Interval { 5, Quality::Perfect };
		case (8) : return Interval { 6, Quality::Minor };
		case (9) : return Interval { 6, Quality::Major };
		case (10) : return Interval { 7, Quality::Minor };
		case (11) : return Interval { 7, Quality::Major };
		case (12) : return Interval { 8, Quality::Perfect };
		case (13) : return Interval { 8, Quality::Augmented };
	}

	jassertfalse;
	return Interval { 0, Quality::Perfect };
}

constexpr Interval Interval::fromPitches (int midiPitch1, int midiPitch2) noexcept
{
	return fromNumSemitones (midiPitch1 - midiPitch2);
}

constexpr Interval::Interval (const Interval& other) noexcept
	: Interval (other.kind, other.quality)
{
}

constexpr Interval& Interval::operator= (const Interval& other) noexcept
{
	kind	= other.kind;
	quality = other.quality;
	return *this;
}

constexpr Interval Interval::operator+ (const Interval& other) const noexcept
{
	return fromNumSemitones (getNumSemitones() + other.getNumSemitones());
}

constexpr Interval Interval::operator- (const Interval& other) const noexcept
{
	return fromNumSemitones (getNumSemitones() - other.getNumSemitones());
}

constexpr Interval Interval::operator+ (int semitonesToAdd) const noexcept
{
	return fromNumSemitones (getNumSemitones() + semitonesToAdd);
}

constexpr Interval Interval::operator- (int semitonesToSubtract) const noexcept
{
	return fromNumSemitones (getNumSemitones() - semitonesToSubtract);
}

constexpr bool Interval::operator== (const Interval& other) const noexcept
{
	return kind == other.kind && quality == other.quality;
}

constexpr bool Interval::operator!= (const Interval& other) const noexcept
{
	return ! (*this == other);
}

constexpr bool Interval::isEnharmonicTo (const Interval& other) const noexcept
{
	return getNumSemitones() == other.getNumSemitones();
}

constexpr bool Interval::operator> (const Interval& other) const noexcept
{
	if (getNumSemitones() > other.getNumSemitones())
		return true;

	return kind > other.kind;
}

constexpr bool Interval::operator< (const Interval& other) const noexcept
{
	if (getNumSemitones() < other.getNumSemitones())
		return true;

	return kind < other.kind;
}

constexpr Interval& Interval::operator++() noexcept
{
	if (kind == 8 && quality == Quality::Augmented)
		return *this;

	auto incrementKind = [this]
	{
		const auto wasFourth = kind == 4;

		++kind;

		if (kind == 1) kind = 2;

		if (intervalIsPerfectKind())
		{
			if (wasFourth)
				quality = Quality::Perfect;
			else
				quality = Quality::Augmented;
		}
		else
		{
			quality = Quality::Major;
		}
	};

	if (intervalIsPerfectKind())
	{
		switch (quality)
		{
			case (Quality::Perfect) :
			{
				quality = Quality::Augmented;
				return *this;
			}
			case (Quality::Augmented) :
			{
				incrementKind();
				return *this;
			}
			case (Quality::Diminished) :
			{
				quality = Quality::Perfect;
				return *this;
			}
			default : jassertfalse; return *this;
		}
	}

	switch (quality)
	{
		case (Quality::Major) :
		{
			quality = Quality::Augmented;
			return *this;
		}
		case (Quality::Minor) :
		{
			quality = Quality::Major;
			return *this;
		}
		case (Quality::Augmented) :
		{
			incrementKind();
			return *this;
		}
		case (Quality::Diminished) :
		{
			quality = Quality::Minor;
			return *this;
		}
		default : jassertfalse; return *this;
	}
}

constexpr Interval& Interval::operator--() noexcept
{
	if (kind == 0 && quality == Quality::Perfect)
		return *this;

	if (kind == 2 && quality == Quality::Diminished)
	{
		kind	= 0;
		quality = Quality::Perfect;
		return *this;
	}

	auto decrementKind = [this]
	{
		const auto wasFifth = kind == 5;

		--kind;

		if (kind == 1) kind = 0;

		if (intervalIsPerfectKind())
		{
			if (wasFifth)
				quality = Quality::Perfect;
			else
				quality = Quality::Diminished;
		}
		else
		{
			quality = Quality::Minor;
		}
	};

	if (intervalIsPerfectKind())
	{
		switch (quality)
		{
			case (Quality::Perfect) :
			{
				quality = Quality::Diminished;
				return *this;
			}
			case (Quality::Augmented) :
			{
				quality = Quality::Perfect;
				return *this;
			}
			case (Quality::Diminished) :
			{
				decrementKind();
				return *this;
			}
			default : jassertfalse; return *this;
		}
	}

	switch (quality)
	{
		case (Quality::Major) :
		{
			quality = Quality::Minor;
			return *this;
		}
		case (Quality::Minor) :
		{
			quality = Quality::Diminished;
			return *this;
		}
		case (Quality::Augmented) :
		{
			quality = Quality::Major;
			return *this;
		}
		case (Quality::Diminished) :
		{
			decrementKind();
			return *this;
		}
		default : jassertfalse; return *this;
	}
}

constexpr int Interval::getNumSemitones() const noexcept
{
	auto major_or_minor = [q = quality] (int baseSemitones)
	{
		switch (q)
		{
			case (Quality::Major) : return baseSemitones;
			case (Quality::Minor) : return baseSemitones - 1;
			case (Quality::Augmented) : return baseSemitones + 1;
			case (Quality::Diminished) : return baseSemitones - 2;
			default : jassertfalse; return baseSemitones;
		}
	};

	auto perfect = [q = quality] (int baseSemitones)
	{
		switch (q)
		{
			case (Quality::Perfect) : return baseSemitones;
			case (Quality::Augmented) : return baseSemitones + 1;
			case (Quality::Diminished) : return baseSemitones - 1;
			default : jassertfalse; return baseSemitones;
		}
	};

	switch (kind)
	{
		case (0) : return perfect (0);
		case (2) : return major_or_minor (2);
		case (3) : return major_or_minor (4);
		case (4) : return perfect (5);
		case (5) : return perfect (7);
		case (6) : return major_or_minor (9);
		case (7) : return major_or_minor (11);
		case (8) : return perfect (12);
	}

	jassertfalse;
	return 0;
}

constexpr int Interval::getKind() const noexcept
{
	return kind;
}

constexpr Interval::Quality Interval::getQuality() const noexcept
{
	return quality;
}

constexpr int Interval::getCents_EqualTemperament() const noexcept
{
	return getNumSemitones() * 100;
}

constexpr Interval Interval::getInverse() const noexcept
{
	switch (getNumSemitones())
	{
		case (0) :
		{
			switch (kind)
			{
				case (0) : return Interval { 8, Quality::Perfect };
				case (2) : return Interval { 7, Quality::Augmented };
			}
		}
		case (1) :
		{
			switch (kind)
			{
				case (0) : return Interval { 8, Quality::Diminished };
				case (2) : return Interval { 7, Quality::Major };
			}
		}
		case (2) :
		{
			switch (kind)
			{
				case (2) : return Interval { 7, Quality::Minor };
				case (3) : return Interval { 6, Quality::Augmented };
			}
		}
		case (3) :
		{
			switch (kind)
			{
				case (2) : return Interval { 7, Quality::Diminished };
				case (3) : return Interval { 6, Quality::Major };
			}
		}
		case (4) :
		{
			switch (kind)
			{
				case (3) : return Interval { 6, Quality::Minor };
				case (4) : return Interval { 5, Quality::Augmented };
			}
		}
		case (5) :
		{
			switch (kind)
			{
				case (3) : return Interval { 6, Quality::Diminished };
				case (4) : return Interval { 5, Quality::Perfect };
			}
		}
		case (6) :
		{
			switch (kind)
			{
				case (4) : return Interval { 5, Quality::Diminished };
				case (5) : return Interval { 4, Quality::Augmented };
			}
		}
		case (7) :
		{
			switch (kind)
			{
				case (5) : return Interval { 4, Quality::Perfect };
				case (6) : return Interval { 3, Quality::Augmented };
			}
		}
		case (8) :
		{
			switch (kind)
			{
				case (5) : return Interval { 4, Quality::Diminished };
				case (6) : return Interval { 3, Quality::Major };
			}
		}
		case (9) :
		{
			switch (kind)
			{
				case (6) : return Interval { 3, Quality::Minor };
				case (7) : return Interval { 2, Quality::Augmented };
			}
		}
		case (10) :
		{
			switch (kind)
			{
				case (6) : return Interval { 3, Quality::Diminished };
				case (7) : return Interval { 2, Quality::Major };
			}
		}
		case (11) :
		{
			switch (kind)
			{
				case (7) : return Interval { 2, Quality::Minor };
				case (8) : return Interval { 0, Quality::Augmented };
			}
		}
		case (12) :
		{
			switch (kind)
			{
				case (7) : return Interval { 2, Quality::Diminished };
				case (8) : return Interval { 0, Quality::Perfect };
			}
		}
		case (13) :
			return Interval { 0, Quality::Augmented };
	}

	jassertfalse;
	return {};
}

}  // namespace lemons::music
