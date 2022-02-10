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

/** A class for representing intervals larger than an octave.
	Internally, the interval is represented as a number of octaves and an Interval object.
	If you know your interval is going to be smaller than an octave, prefer to use the Interval class directly.
 */
class CompoundInterval
{
public:

	// TO DO: to/from string description

	/** Creates a compound interval with the given number of octaves and the given simple interval component.
		If the simple interval is a perfect or augmented octave, the number of octaves will be incremented and the simple interval component will be reduced to a perfect or augmented unison.
	 */
	constexpr explicit CompoundInterval (int octaves, const Interval& simpleInterval) noexcept
		: interval (simpleInterval), numOctaves (octaves)
	{
		if (interval.getKind() == 8)
		{
			const auto quality = interval.getQuality();

			if (quality != Interval::Quality::Diminished)
			{
				++numOctaves;
				interval = Interval { 0, quality };
			}
		}
	}

	/** Constructs a compound interval with the given number of semitones.
	 */
	constexpr explicit CompoundInterval (int numSemitones) noexcept
	{
		numOctaves = numSemitones / 12;
		interval   = Interval::fromNumSemitones (numSemitones - (numOctaves * 12));
	}

	/** Returns a compound interval object representing the distance between two MIDI notes.
	 */
	[[nodiscard]] static constexpr CompoundInterval fromPitches (int midiPitch1, int midiPitch2) noexcept
	{
		return CompoundInterval { math::abs (midiPitch2 - midiPitch1) };
	}

	/** Returns a compound interval object representing the distance between two pitch objects. */
	[[nodiscard]] static CompoundInterval fromPitches (const Pitch& pitch1, const Pitch& pitch2) noexcept;

	/** Copy constructor. */
	constexpr CompoundInterval (const CompoundInterval& other) noexcept
		: interval (other.interval), numOctaves (other.numOctaves)
	{
	}

	/** Returns true if the two compound intervals are exactly equal; that is, have the same number of octaves, and their simple interval components are exactly equal in kind and quality. */
	[[nodiscard]] constexpr bool operator== (const CompoundInterval& other) noexcept
	{
		return numOctaves == other.numOctaves && interval == other.interval;
	}

	/** Returns true if the two intervals represent the same number of semitones, regardless of their enharmonic spelling. */
	[[nodiscard]] constexpr bool isEnharmonicTo (const CompoundInterval& other) noexcept
	{
		return getNumSemitones() == other.getNumSemitones();
	}

	/** Returns true if the two intervals are not exactly equal. */
	[[nodiscard]] constexpr bool operator!= (const CompoundInterval& other) noexcept
	{
		return ! (*this == other);
	}

	/** Returns true if this interval is semantically larger than the other one, taking enharmonic spellings into account. */
	[[nodiscard]] constexpr bool operator> (const CompoundInterval& other) noexcept
	{
		if (numOctaves < other.numOctaves)
			return false;

		return interval > other.interval;
	}

	/** Returns true if this interval is semantically smaller than the other one, taking enharmonic spellings into account. */
	[[nodiscard]] constexpr bool operator< (const CompoundInterval& other) noexcept
	{
		if (numOctaves > other.numOctaves)
			return false;

		return interval < other.interval;
	}

	/** Returns the number of octaves contained in this compound interval. */
	[[nodiscard]] constexpr int getNumOctaves() const noexcept
	{
		return numOctaves;
	}

	/** Returns the simple interval component of this compound interval. The interval object returned by this function will always be a diminished octave or smaller, and may be a perfect unison. */
	[[nodiscard]] constexpr Interval getSimpleInterval() const noexcept
	{
		return interval;
	}

	/** Returns the number of semitones this compound interval represents. */
	[[nodiscard]] constexpr int getNumSemitones() const noexcept
	{
		return interval.getNumSemitones() + (numOctaves * 12);
	}

	/** Returns the quality of this compound interval.
		The quality of compound intervals is determined by the quality of their simple interval component; this is identical to calling:
		@code
		compoundInterval.getSimpleInterval().getQuality();
		@endcode
	 */
	[[nodiscard]] constexpr Interval::Quality getQuality() const noexcept
	{
		return interval.getQuality();
	}

	/** Returns the kind of this compound interval as an integer, eg, 9, 10, etc.
	 */
	[[nodiscard]] constexpr int getKind() const noexcept
	{
		const auto simple = [kind = interval.getKind()]
		{
			if (kind > 0)
				return kind - 1;

			return kind;
		}();

		return (numOctaves * 7) + simple + 1;
	}

	/** Returns a string description of this compound interval.
		@param useShort When true, a shorthand version of the description will be returned -- for example, "M9" instead of "Major ninth", etc.
	 */
	[[nodiscard]] String getStringDescription (bool useShort = false) const;

	/** Returns the number of cents this interval represents in equal temperament. */
	[[nodiscard]] constexpr int getCents_EqualTemperament() const noexcept
	{
		return getNumSemitones() * 100;
	}

	/** Returns an array of possible cents values this interval can represent in just intonation, taking enharmonic spelling into account.
	 */
	[[nodiscard]] juce::Array<double> getCents_JustIntonation() const;

private:

	Interval interval;

	int numOctaves { 0 };
};

}  // namespace lemons::music
