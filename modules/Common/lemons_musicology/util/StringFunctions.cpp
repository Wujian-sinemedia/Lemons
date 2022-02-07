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

const juce_wchar getSharpSymbol() noexcept
{
	return *juce::CharPointer_UTF8 ("\xe2\x99\xaf");
}

const juce_wchar getFlatSymbol() noexcept
{
	return *juce::CharPointer_UTF8 ("\xe2\x99\xad");
}

const juce_wchar getNaturalSymbol() noexcept
{
	return *juce::CharPointer_UTF8 ("\xe2\x99\xae");
}

int stringToPitchClass (const String& string) noexcept
{
	jassert (! string.isEmpty());

	const auto pitchClassTokens = juce::String ("abcdefg#") + getSharpSymbol() + getFlatSymbol() + getNaturalSymbol();
	const auto pitchClassName	= string.toLowerCase().retainCharacters (pitchClassTokens);

	const auto numChars = pitchClassName.length();

	auto pitchClass = [&]() -> int
	{
		if (numChars > 0)
		{
			const auto base = pitchClassName.toLowerCase()[0];

			switch (base)
			{
				case 'c' : return 0;
				case 'd' : return 2;
				case 'e' : return 4;
				case 'f' : return 5;
				case 'g' : return 7;
				case 'a' : return 9;
				case 'b' : return 11;
				default : return -1;
			}
		}

		return -1;
	}();

	if (numChars > 1 && pitchClass >= 0)
	{
		const auto sharpOrFlat = pitchClassName[1];

		if (sharpOrFlat == getSharpSymbol() || sharpOrFlat == '#')
			++pitchClass;
		else if (sharpOrFlat == getFlatSymbol() || sharpOrFlat == 'b')
			--pitchClass;

		pitchClass %= 12;
	}

	return pitchClass;
}


static const char* const sharpNoteNames[] = {
	"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"
};

static const char* const flatNoteNames[] = {
	"C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B"
};

String pitchClassToString (int pitchClass, bool asSharps) noexcept
{
	jassert (pitchClass >= 0 && pitchClass <= 11);

	if (asSharps)
		return sharpNoteNames[pitchClass];

	return flatNoteNames[pitchClass];
}


String pitchToString (int midiNoteNumber, bool asSharps) noexcept
{
	jassert (midiNoteNumber >= 0 && midiNoteNumber <= 127);

	const auto noteName = pitchClassToString (midiNoteNumber % 12, asSharps);

	const auto octave = midiNoteNumber / 12 - 1;

	return noteName + String (octave);
}

int stringToPitch (const String& string) noexcept
{
	jassert (! string.isEmpty());

	const auto pitchClass = stringToPitchClass (string);

	if (pitchClass < 0)
		return -1;

	const auto octave = string.retainCharacters ("0123456789").getIntValue();

	return (octave * 12) + pitchClass + 12;
}


String midiPanIntToString (int midiPan) noexcept
{
	jassert (midiPan >= 0 && midiPan <= 127);

	if (midiPan == 64) return TRANS ("C");

	if (midiPan > 64)
	{
		const auto amtRight = juce::jmap (midiPan, 65, 127, 1, 50);
		return String (amtRight) + TRANS ("R");
	}

	const auto amtLeft = juce::jmap (midiPan, 63, 0, 1, 50);
	return String (amtLeft) + TRANS ("L");
}

int midiPanStringToInt (const String& string) noexcept
{
	jassert (! string.isEmpty());

	if (string.endsWithIgnoreCase (TRANS ("C"))) return 64;

	if (string.endsWithIgnoreCase (TRANS ("R")))
		return juce::jmap (string.dropLastCharacters (1).getIntValue(),
						   1, 50, 65, 127);

	if (string.endsWithIgnoreCase (TRANS ("L")))
		return juce::jmap (string.dropLastCharacters (1).getIntValue(),
						   1, 50, 63, 0);

	jassertfalse;
	return string.getIntValue();
}


}  // namespace lemons::music
