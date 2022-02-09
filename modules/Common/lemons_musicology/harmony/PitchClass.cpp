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

String PitchClass::getAsString (bool asSharps, bool useUnicodeAccidentals, bool useNaturalSymbol) const noexcept
{
	if (useNaturalSymbol)
		jassert (useUnicodeAccidentals);

	const constexpr char* const sharpNoteNames[] = {
		"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"
	};

	const constexpr char* const flatNoteNames[] = {
		"C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B"
	};

	const auto base = [sharps = asSharps, idx = pitchClass, &sharpNoteNames, &flatNoteNames]
	{
		if (sharps)
			return sharpNoteNames[idx];

		return flatNoteNames[idx];
	}();

	if (! useUnicodeAccidentals)
		return base;

	if (! isBlackKey())
	{
		if (useNaturalSymbol)
			return base + getNaturalSymbol();

		return base;
	}

	if (asSharps)
		return sharpNoteNames[pitchClass - 1] + getSharpSymbol();

	return flatNoteNames[pitchClass + 1] + getFlatSymbol();
}


inline int stringToPitchClass (const String& string) noexcept
{
	jassert (! string.isEmpty());

	const auto pitchClassTokens = String ("abcdefg#") + getSharpSymbol() + getFlatSymbol() + getNaturalSymbol();

	const auto pitchClassName = string.trim().toLowerCase().retainCharacters (pitchClassTokens);

	const auto numChars = pitchClassName.length();

	jassert (numChars > 0);

	const auto pitchClass = [numChars, &pitchClassName]()
	{
		if (numChars > 0)
		{
			switch (pitchClassName.toLowerCase()[0])
			{
				case 'c' : return 0;
				case 'd' : return 2;
				case 'e' : return 4;
				case 'f' : return 5;
				case 'g' : return 7;
				case 'a' : return 9;
				case 'b' : return 11;
			}
		}

		return -1;
	}();

	if (pitchClass < 0)
	{
		jassertfalse;
		return -1;
	}

	if (numChars > 1)
	{
		const auto sharpOrFlat = pitchClassName[1];

		if (sharpOrFlat == getSharpSymbol() || sharpOrFlat == '#')
			return pitchClass + 1;

		if (sharpOrFlat == getFlatSymbol() || sharpOrFlat == 'b')
			return pitchClass - 1;
	}

	return pitchClass;
}

PitchClass::PitchClass (const String& stringDescription) noexcept
	: PitchClass (stringToPitchClass (stringDescription))
{
}

}  // namespace lemons::music
