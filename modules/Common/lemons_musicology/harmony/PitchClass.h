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

struct PitchClass final
{
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

	constexpr PitchClass (const PitchClass& other) noexcept
		: pitchClass (other.pitchClass)
	{
	}

	[[nodiscard]] constexpr bool operator== (const PitchClass& other) const noexcept
	{
		return pitchClass == other.pitchClass;
	}

	[[nodiscard]] constexpr bool operator!= (const PitchClass& other) const noexcept
	{
		return pitchClass != other.pitchClass;
	}

	[[nodiscard]] constexpr bool isBlackKey() const noexcept
	{
		return pitchClass == 1 || pitchClass == 3 || pitchClass == 6 || pitchClass == 8 || pitchClass == 10;
	}

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

	constexpr operator int() const noexcept
	{
		return pitchClass;
	}

	[[nodiscard]] constexpr int getAsInt() const noexcept
	{
		return pitchClass;
	}

private:

	const int pitchClass { 0 };
};

}  // namespace lemons::music
