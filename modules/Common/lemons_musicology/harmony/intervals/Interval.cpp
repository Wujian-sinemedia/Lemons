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

Interval Interval::fromPitches (const Pitch& pitch1, const Pitch& pitch2) noexcept
{
	return fromNumSemitones (pitch1.getRoundedMidiPitch() - pitch2.getRoundedMidiPitch());
}

String Interval::getKindAsString() const
{
	switch (kind)
	{
		case (0) : return "unison";
		case (2) : return "second";
		case (3) : return "third";
		case (4) : return "fourth";
		case (5) : return "fifth";
		case (6) : return "sixth";
		case (7) : return "seventh";
		case (8) : return "octave";
	}

	jassertfalse;
	return "";
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
		}
	}

	switch (quality)
	{
		case (Quality::Major) : return "Major";
		case (Quality::Minor) : return "Minor";
		case (Quality::Perfect) : return "Perfect";
		case (Quality::Augmented) : return "Augmented";
		case (Quality::Diminished) : return "Diminished";
	}

	jassertfalse;
	return "";
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

Pitch Interval::applyToPitch (const Pitch& other, bool above) const noexcept
{
	const auto semitones = getNumSemitones();
	const auto starting	 = other.getRoundedMidiPitch();

	if (above)
		return Pitch { starting + semitones };

	return Pitch { starting - semitones };
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
		case (13) :
			return { 1271. };
	}

	jassertfalse;
	return {};
}

juce::Array<math::Fraction<int>> Interval::getJustRatios() const
{
	using F = math::Fraction<int>;

	switch (getNumSemitones())
	{
		case (0) :
		{
			switch (kind)
			{
				case (0) : return {
					F {1, 1}
 };
				case (2) : return {
					F {128, 125}
 };
			}
		}
		case (1) :
		{
			switch (kind)
			{
				case (0) : return {
					F {25,	24  },
					F { 2187, 2048}
 };
				case (2) : return {
					F {16, 15}
 };
			}
		}
		case (2) :
		{
			switch (kind)
			{
				case (2) : return {
					F {9,   8},
					F { 10, 9}
 };
				case (3) : return {
					F {144,	 125	 },
					F { 256,	 225	 },
					F { 65536, 59049}
 };
			}
		}
		case (3) :
		{
			switch (kind)
			{
				case (2) : return {
					F {75,		64   },
					F { 125,	 108	 },
					F { 19,	16   },
					F { 19683, 16384}
 };
				case (3) : return {
					F {6,   5 },
					F { 19, 16},
					F { 32, 27}
 };
			}
		}
		case (4) :
		{
			switch (kind)
			{
				case (3) : return {
					F {5,   4 },
					F { 81, 64},
					F { 9,  7 }
 };
				case (4) : return {
					F {32,	25  },
					F { 8192, 6561}
 };
			}
		}
		case (5) :
		{
			switch (kind)
			{
				case (3) : return {
					F {125, 96},
					F { 21, 16},
					F { 64, 49}
 };
				case (4) : return {
					F {4, 3}
 };
			}
		}
		case (6) :
		{
			return {
				F {729,	 512},
				F { 1024, 729},
				F { 25,	18 },
				F { 36,	25 },
				F { 45,	32 },
				F { 64,	45 },
				F { 7,	   5	 },
				F { 10,	7  },
				F { 13,	9  },
				F { 18,	13 }
			};
		}
		case (7) :
		{
			switch (kind)
			{
				case (5) : return {
					F {3, 2}
 };
				case (6) : return {
					F {192, 125},
					F { 32, 21 },
					F { 49, 32 }
 };
			}
		}
		case (8) :
		{
			switch (kind)
			{
				case (5) : return {
					F {25,	16  },
					F { 6561, 4096}
 };
				case (6) : return {
					F {8,	   5 },
					F { 128, 81},
					F { 11,	7 }
 };
			}
		}
		case (9) :
		{
			switch (kind)
			{
				case (6) : return {
					F {5,   3 },
					F { 12, 7 },
					F { 27, 16}
 };
				case (7) : return {
					F {128,	 75	},
					F { 216,	 125	 },
					F { 32772, 19683}
 };
			}
		}
		case (10) :
		{
			switch (kind)
			{
				case (6) : return {
					F {125,	 72	},
					F { 225,	 128	 },
					F { 7,	   4	 },
					F { 59049, 32768}
 };
				case (7) : return {
					F {16, 9},
					F { 9, 5}
 };
			}
		}
		case (11) :
		{
			switch (kind)
			{
				case (7) : return {
					F {15,	8 },
					F { 50, 27}
 };
				case (8) : return {
					F {48,	25  },
					F { 256,	 135 },
					F { 4096, 2187}
 };
			}
		}
		case (12) :
		{
			switch (kind)
			{
				case (7) : return {
					F {125,	 64	},
					F { 2025, 1024}
 };
				case (8) : return {
					F {2, 1}
 };
			}
		}
		case (13) :
			return {
				F {25, 12}
			};
	}

	jassertfalse;
	return {};
}

/*--------------------------------------------------------------------------------------------------------*/

Pitch operator+ (const Pitch& pitch, const Interval& interval) noexcept
{
	return Pitch { pitch.getRoundedMidiPitch() + interval.getNumSemitones() };
}

Pitch operator- (const Pitch& pitch, const Interval& interval) noexcept
{
	return Pitch { pitch.getRoundedMidiPitch() - interval.getNumSemitones() };
}

}  // namespace lemons::music
