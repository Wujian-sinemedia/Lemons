#pragma once

#include <juce_core/juce_core.h>
#include <lemons_core/lemons_core.h>

namespace lemons::music
{

using juce::juce_wchar;
using juce::String;

/** Returns a Unicode sharp symbol. */
[[nodiscard]] const juce_wchar getSharpSymbol() noexcept;

/** Returns a Unicode flat symbol. */
[[nodiscard]] const juce_wchar getFlatSymbol() noexcept;

/** Returns a Unicode natural symbol. */
[[nodiscard]] const juce_wchar getNaturalSymbol() noexcept;


/** Converts a MIDI pitch to a string representation; e.g. '69' would return 'A4'.
	@param midiNoteNumber The MIDI note number to convert to a string representation.
	@param asSharps Whether to use sharps when describing black-key notes; for example, when this is true, '70' would print 'A#4'; when this is false, '70' would print 'Ab4'.
	@see stringToPitch()
 */
[[nodiscard]] String pitchToString (int midiNoteNumber, bool asSharps = true) noexcept;


/** Converts a string description of a pitch to a MIDI note number.
	The string should be in the format 'A#7', 'Bb9', etc. You can also use the Unicode sharp, flat, and natural symbols in the passed string.
	If the passed string is in the wrong format, or it can't be parsed correctly for some reason, this will return -1.
	@see stringToPitchClass(), pitchToString()
 */
[[nodiscard]] int stringToPitch (const String& string) noexcept;


/** Returns a string description of a pitch to an integer representing the pitch class, where C is 0, C# is 1, D is 2... and B is 11.
	The string should be in the format 'A#', 'Bb', etc. You can also use the Unicode sharp, flat, and natural symbols in the passed string.
	If the passed string is in the wrong format, or it can't be parsed correctly for some reason, this will return -1.
	@see pitchClassToString()
 */
[[nodiscard]] int stringToPitchClass (const String& pitchClassName) noexcept;

/** Returns a string description of a pitch class in the range 0-11, with 0 being 'C', and 11 being 'B'.
	@see stringToPitchClass()
 */
[[nodiscard]] String pitchClassToString (int pitchClass, bool asSharps = true) noexcept;


/** Takes an integer in the range 0-127 and creates a panning description string, where 64 prints "C", 65 prints "1R", 127 prints "50R", 63 prints "1L", and 0 prints "50L".
	@see midiPanStringToInt()
 */
[[nodiscard]] String midiPanIntToString (int midiPan) noexcept;


/** Attempts to parse a MIDI pan description string, as described by midiPanIntToString(), and return the original MIDI panning value in the range 0-127.
	@see midiPanIntToString()
 */
[[nodiscard]] int midiPanStringToInt (const String& string) noexcept;

}  // namespace lemons::music


/*---------------------------------------------------------------------------------------------------------------------------------*/

#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

struct StringFunctionsTests : public CoreTest
{
	StringFunctionsTests();

private:
	void runTest() final;
};

static StringFunctionsTests stringFunctionsTest;

}  // namespace lemons::tests

#endif
