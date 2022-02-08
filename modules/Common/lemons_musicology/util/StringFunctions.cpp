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

String pitchToString (int midiNoteNumber, bool asSharps) noexcept
{
	jassert (midiNoteNumber >= 0 && midiNoteNumber <= 127);

	const PitchClass pitchClass { midiNoteNumber };

	const auto noteName = pitchClass.getAsString (asSharps);

	const auto octave = octaveNumberOfMidiNote (midiNoteNumber);

	return noteName + String (octave);
}

int stringToPitch (const String& string) noexcept
{
	jassert (! string.isEmpty());

	const PitchClass pitchClass { string };

	const auto pitchClassInt = pitchClass.getAsInt();

	const auto octave = string.retainCharacters ("0123456789").getIntValue();

	return (octave * 12) + pitchClassInt + 12;
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
