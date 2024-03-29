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

#include <juce_core/juce_core.h>

namespace lemons::music
{

using juce::String;

/** A struct that represents a pitch class.
	This class is agnostic to enharmonic spellings; thus, C-sharp and D-flat are considered the same pitch class.
 */
struct PitchClass final
{
	/** Creates a pitch class from an integer, where 0 is C, 1 is C#/Db, and 11 is B.
	 */
	constexpr explicit PitchClass (int midiPitch) noexcept
		: pitchClass (midiPitch % 12)
	{
	}

	/** Creates a pitch class from a string description of one.
		The string should be in the format 'A#', 'Bb', etc. You can also use the Unicode sharp, flat, and natural symbols in the passed string.
		If the passed string is in the wrong format, or it can't be parsed correctly for some reason, an assertion will be thrown.
		@see getAsString()
	 */
	explicit PitchClass (const String& stringDescription) noexcept;

	/** Copy constructor. */
	constexpr PitchClass (const PitchClass& other) noexcept
		: pitchClass (other.pitchClass)
	{
	}

	/** Assignment operator. */
	constexpr PitchClass& operator= (const PitchClass& other) noexcept
	{
		pitchClass = other.pitchClass;
		return *this;
	}

	/** Returns true if the two pitch classes are equal. */
	[[nodiscard]] constexpr bool operator== (const PitchClass& other) const noexcept
	{
		return pitchClass == other.pitchClass;
	}

	/** Returns true if the two pitch classes are not equal. */
	[[nodiscard]] constexpr bool operator!= (const PitchClass& other) const noexcept
	{
		return pitchClass != other.pitchClass;
	}

	/** Returns true if this pitch class represents a black key on a standard keyboard. */
	[[nodiscard]] constexpr bool isBlackKey() const noexcept
	{
		return pitchClass == 1 || pitchClass == 3 || pitchClass == 6 || pitchClass == 8 || pitchClass == 10;
	}

	/** Returns true if this pitch class represents a white key on a standard keyboard. */
	[[nodiscard]] constexpr bool isWhiteKey() const noexcept
	{
		return ! isBlackKey();
	}

	/** Returns a string description of this pitch class.
		@param asSharps If this is true and this pitch class is a black key, the returned string will use sharps to represent the pitch. Otherwise, flats will be used.
		@param useUnicodeAccidentals If this is true, unicode sharp and natural characters will be used; otherwise, '#' or 'b' will be used for sharp and flat, respectively.
		@param useNaturalSymbol If this pitch class is not a black key and this parameter is true, a unicode natural symbol will be appended to the returned string. This option only has any effect if useUnicodeAccidentals is true.
	 */
	[[nodiscard]] String getAsString (bool asSharps				 = true,
									  bool useUnicodeAccidentals = false,
									  bool useNaturalSymbol		 = false) const noexcept;

	/** Returns this pitch class as an integer between 0 and 11, where 0 is C and 11 is B. */
	constexpr operator int() const noexcept
	{
		return pitchClass;
	}

	/** Returns this pitch class as an integer between 0 and 11, where 0 is C and 11 is B. */
	[[nodiscard]] constexpr int getAsInt() const noexcept
	{
		return pitchClass;
	}

private:

	int pitchClass { 0 };
};

}  // namespace lemons::music
