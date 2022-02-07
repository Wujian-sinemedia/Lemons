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

constexpr bool intervalIsPerfectKind (int kind)
{
	return kind == 0 || kind == 4 || kind == 5 || kind == 8;
}


constexpr Interval::Interval (int kindToUse, Quality qualityToUse) noexcept
	: quality (qualityToUse), kind (kindToUse)
{
	if (intervalIsPerfectKind (kind))
		jassert (quality != Quality::Major && quality != Quality::Minor);
	else
		jassert (quality != Quality::Perfect);
}

constexpr Interval Interval::fromNumSemitones (int semitones) noexcept
{
	semitones %= 12;

	jassert (semitones >= 0 && semitones <= 12);

	switch (semitones)
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
		default : jassertfalse; return Interval { 0, Quality::Perfect };
	}
}

constexpr Interval::Interval (const Interval& other) noexcept
	: Interval (other.kind, other.quality)
{
}

constexpr Interval Interval::operator+ (const Interval& other) const noexcept
{
	return fromNumSemitones (getNumSemitones() + other.getNumSemitones());
}

constexpr Interval Interval::operator- (const Interval& other) const noexcept
{
	return fromNumSemitones (std::abs (getNumSemitones() - other.getNumSemitones()));
}

constexpr Interval Interval::operator+ (int semitonesToAdd) const noexcept
{
	return fromNumSemitones (getNumSemitones() + semitonesToAdd);
}

constexpr Interval Interval::operator- (int semitonesToSubtract) const noexcept
{
	return fromNumSemitones (std::abs (getNumSemitones() - semitonesToSubtract));
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
	return getNumSemitones() > other.getNumSemitones();
}

constexpr bool Interval::operator< (const Interval& other) const noexcept
{
	return getNumSemitones() < other.getNumSemitones();
}

constexpr Interval& Interval::operator++() noexcept
{
	auto incrementKind = [this]
	{
		++kind;

		if (intervalIsPerfectKind (kind))
			quality = Quality::Perfect;
		else
			quality = Quality::Major;
	};

	if (intervalIsPerfectKind (kind))
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
	auto decrementKind = [this]
	{
		--kind;

		if (intervalIsPerfectKind (kind))
			quality = Quality::Perfect;	 // ??
		else
			quality = Quality::Minor;
	};

	if (intervalIsPerfectKind (kind))
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
		default : jassertfalse; return 0;
	}
}

constexpr int Interval::getKind() const noexcept
{
	return kind;
}

String Interval::getKindAsString() const
{
	jassert (kind >= 0 && kind <= 8);

	switch (kind)
	{
		case (0) : return "unison";
		case (1) : jassertfalse; return "";
		case (2) : return "second";
		case (3) : return "third";
		case (4) : return "fourth";
		case (5) : return "fifth";
		case (6) : return "sixth";
		case (7) : return "seventh";
		case (8) : return "octave";
		default : jassertfalse; return "";
	}
}

constexpr Interval::Quality Interval::getQuality() const noexcept
{
	return quality;
}

String Interval::getQualityAsString (bool useShort) const
{
	if (useShort)
	{
		switch (quality)
		{
			case (Quality::Major) : return "M";
			case (Quality::Minor) : return "m";
			case (Quality::Perfect) : return "P";
			case (Quality::Augmented) : return "A";
			case (Quality::Diminished) : return "d";
			default : jassertfalse; return "";
		}
	}

	switch (quality)
	{
		case (Quality::Major) : return "Major";
		case (Quality::Minor) : return "Minor";
		case (Quality::Perfect) : return "Perfect";
		case (Quality::Augmented) : return "Augmented";
		case (Quality::Diminished) : return "Diminished";
		default : jassertfalse; return "";
	}
}

String Interval::getStringDescription (bool useShort) const
{
	if (useShort)
		return getQualityAsString (true) + String (kind);

	return getQualityAsString (false) + " " + getKindAsString();
}

Interval Interval::fromStringDescription (const String& string)
{
	const auto trimmed = string.trim();

	if (trimmed.length() == 2)
	{
		const auto kind = String (trimmed.getLastCharacter()).getIntValue();

		const auto qualityChar = trimmed[0];

		if (qualityChar == 'M')
			return Interval { kind, Quality::Major };

		if (qualityChar == 'm')
			return Interval { kind, Quality::Minor };

		if (qualityChar == 'P' || qualityChar == 'p')
			return Interval { kind, Quality::Perfect };

		if (qualityChar == 'A' || qualityChar == 'a')
			return Interval { kind, Quality::Augmented };

		if (qualityChar == 'D' || qualityChar == 'd')
			return Interval { kind, Quality::Diminished };

		jassertfalse;
		return Interval { kind, Quality::Major };
	}

	const auto quality = [qualityString = trimmed.upToFirstOccurrenceOf (" ", false, false)]
	{
		if (qualityString.equalsIgnoreCase ("major"))
			return Interval::Quality::Major;

		if (qualityString.equalsIgnoreCase ("minor"))
			return Interval::Quality::Minor;

		if (qualityString.equalsIgnoreCase ("perfect"))
			return Interval::Quality::Perfect;

		if (qualityString.equalsIgnoreCase ("augmented"))
			return Interval::Quality::Augmented;

		if (qualityString.equalsIgnoreCase ("diminished"))
			return Interval::Quality::Diminished;

		jassertfalse;

		return Interval::Quality::Major;
	}();

	const auto kind = [kindString = trimmed.fromFirstOccurrenceOf (" ", false, false)]
	{
		if (kindString.equalsIgnoreCase ("unison"))
			return 0;

		if (kindString.equalsIgnoreCase ("second"))
			return 2;

		if (kindString.equalsIgnoreCase ("third"))
			return 3;

		if (kindString.equalsIgnoreCase ("fourth"))
			return 4;

		if (kindString.equalsIgnoreCase ("fifth"))
			return 5;

		if (kindString.equalsIgnoreCase ("sixth"))
			return 6;

		if (kindString.equalsIgnoreCase ("seventh"))
			return 7;

		if (kindString.equalsIgnoreCase ("octave"))
			return 8;

		jassertfalse;

		return 0;
	}();

	return Interval { kind, quality };
}

constexpr int Interval::getCents_EqualTemperament() const noexcept
{
	return getNumSemitones() * 100;
}

juce::Array<double> Interval::getCents_JustIntonation() const
{
	switch (getNumSemitones())
	{
		case (0) :
		{
			switch (kind)
			{
				case (0) : return { 0. };
				case (2) : return { 41.1 };
			}
		}
		case (1) :
		{
			switch (kind)
			{
				case (0) : return { 71., 114. };
				case (2) : return { 100. };
			}
		}
		case (2) :
		{
			switch (kind)
			{
				case (2) : return { 182., 204. };
				case (3) : return { 180., 223., 245. };
			}
		}
		case (3) :
		{
			switch (kind)
			{
				case (2) : return { 253., 275., 298., 318. };
				case (3) : return { 294., 298., 316. };
			}
		}
		case (4) :
		{
			switch (kind)
			{
				case (3) : return { 386., 408., 435. };
				case (4) : return { 427., 384. };
			}
		}
		case (5) :
		{
			switch (kind)
			{
				case (3) : return { 457. };
				case (4) : return { 498. };
			}
		}
		case (6) :
		{
			return { 563., 569., 583., 588., 590., 610., 612., 617., 631., 637. };
		}
		case (7) :
		{
			switch (kind)
			{
				case (5) : return { 701.955 };
				case (6) : return { 743. };
			}
		}
		case (8) :
		{
			switch (kind)
			{
				case (5) : return { 773., 816. };
				case (6) : return { 782., 792., 814. };
			}
		}
		case (9) :
		{
			switch (kind)
			{
				case (6) : return { 884., 906., 933. };
				case (7) : return { 882., 925., 947. };
			}
		}
		case (10) :
		{
			switch (kind)
			{
				case (6) : return { 955., 969., 977., 1020. };
				case (7) : return { 996., 1018. };
			}
		}
		case (11) :
		{
			switch (kind)
			{
				case (7) : return { 1067., 1088. };
				case (8) : return { 1086., 1108., 1129. };
			}
		}
		case (12) :
		{
			switch (kind)
			{
				case (7) : return { 1159., 1180. };
				case (8) : return { 1200. };
			}
		}
	}

	jassertfalse;
	return {};
}

}  // namespace lemons::music
