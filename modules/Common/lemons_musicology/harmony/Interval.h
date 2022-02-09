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

/** A class that represents a musical interval between two pitches.
	This class is meant for intervals that are an octave or smaller, and that can be expressed as a whole number of semitones.
 */
class Interval final
{
public:

	/** Describes the quality of the interval.
		Note that not all possible quality values are valid for every possible interval type; a "major fifth" or a "perfect third" do not exist.
		@see isValidQualityForKind()
	 */
	enum class Quality
	{
		Diminished,
		Minor,
		Major,
		Augmented,
		Perfect
	};

	/** Creates an interval representing a perfect unison. */
	constexpr Interval();

	/** Creates an interval with a specified kind and quality.
		If the kind or quality parameters are invalid, an assertion will be thrown.
		@param kindToUse The kind of interval to create -- 0 for unison, 2 for second, 3 for third, etc. Note that 1 is not a valid value, and values greater than 8 will be reduced to their equivalent intervals that are within one octave (ie, using the modulus operator).
		@param qualityToUse The quality of interval to create.
		@see isValidQualityForKind()
	 */
	constexpr explicit Interval (int kindToUse, Quality qualityToUse) noexcept;

	/** Creates an interval from a number of semitones.
		This uses reasonable defaults, always preferring major, minor, or perfect intervals when available instead of augmented or diminished.
	 */
	[[nodiscard]] static constexpr Interval fromNumSemitones (int semitones) noexcept;

	/** Creates an interval from the distance between two pitch objects. */
	[[nodiscard]] static Interval fromPitches (const Pitch& pitch1, const Pitch& pitch2) noexcept;

	/** Creates an interval from the distance between two MIDI pitches. */
	[[nodiscard]] static constexpr Interval fromPitches (int midiPitch1, int midiPitch2) noexcept;

	/** Copy constructor. */
	constexpr Interval (const Interval& other) noexcept;

	/** Creates an interval from a string description of one.
		The passed string can be a long or short description, eg, "Major second" or "M2", etc.
	 */
	[[nodiscard]] static Interval fromStringDescription (const String& string);

	/** Returns true if the two intervals are exactly equal -- that is, have the same kind and quality.
		Note that this will return false for intervals that represent the same number of semitones but are spelled differently.
		@see isEnharmonicTo()
	 */
	[[nodiscard]] constexpr bool operator== (const Interval& other) const noexcept;

	/** Returns true if the two intervals represent the same number of semitones, regardless of their enharmonic spelling.
	 */
	[[nodiscard]] constexpr bool isEnharmonicTo (const Interval& other) const noexcept;

	/** Returns true if the two intervals are not exactly equal.
		Note that this will return true for intervals that represent the same number of semitones but are spelled differently.
		@see isEnharmonicTo()
	 */
	[[nodiscard]] constexpr bool operator!= (const Interval& other) const noexcept;

	/** Returns true if this interval is semantically larger than the other one, taking enharmonic spellings into account.
		For example, this will tell you that a diminished fifth is "larger" than an augmented fourth.
	 */
	[[nodiscard]] constexpr bool operator> (const Interval& other) const noexcept;

	/** Returns true if this interval is semantically smaller than the other one, taking enharmonic spellings into account.
		For example, this will tell you that an augmented fourth is "smaller" than a diminished fifth.
	 */
	[[nodiscard]] constexpr bool operator< (const Interval& other) const noexcept;

	/** Adds another interval object to this one.
		@see fromNumSemitones()
	 */
	[[nodiscard]] constexpr Interval operator+ (const Interval& other) const noexcept;

	/** Subtracts another interval object from this one.
		@see fromNumSemitones()
	 */
	[[nodiscard]] constexpr Interval operator- (const Interval& other) const noexcept;

	/** Adds a number of semitones to this interval.
		@see fromNumSemitones()
	 */
	[[nodiscard]] constexpr Interval operator+ (int semitonesToAdd) const noexcept;

	/** Subtracts a number of semitones from this interval.
		@see fromNumSemitones()
	 */
	[[nodiscard]] constexpr Interval operator- (int semitonesToSubtract) const noexcept;

	/** Increments this interval by one semitone, preserving the kind by manipulating the quality where possible.
		For example, a major second will increment to an augmented second, rather than a minor third.
		A perfect octave will increment to an augmented octave.
		Calling the increment operator on an augmented octave will do nothing.
	 */
	constexpr Interval& operator++() noexcept;

	/** Decrements this interval by one semitone, preserving the kind by manipulating the quality where possible.
		For example, a minor second will decrement to a diminished second, rather than a perfect unison.
		In the special case of a diminished second, it will decrement to a perfect unison (even though these two intervals are enharmonically equivalent).
		Calling the decrement operator on a perfect unison will do nothing.
	 */
	constexpr Interval& operator--() noexcept;

	/** Returns the interval that is the inverse of this one, taking enharmonic spellings into account.
		An interval and its inverse add together to form a perfect octave -- for example, a major second's inverse is a minor seventh, a perfect fourth's inverse is a perfect fifth, etc.
	 */
	[[nodiscard]] constexpr Interval getInverse() const noexcept;

	/** Returns the number of semitones that this interval represents. */
	[[nodiscard]] constexpr int getNumSemitones() const noexcept;

	/** Returns the kind of this interval as an integer.
		0 represents unison, 2 is a second, 3 is a third, etc.
	 */
	[[nodiscard]] constexpr int getKind() const noexcept;

	/** Returns a string description of this interval's kind, eg, "second", "third", etc. */
	[[nodiscard]] String getKindAsString() const;

	/** Returns the quality of this interval. */
	[[nodiscard]] constexpr Quality getQuality() const noexcept;

	/** Returns a string description of this interval's quality. */
	[[nodiscard]] String getQualityAsString (bool useShort = false) const;

	/** Returns a string description of this interval.
		@param useShort When true, a shorthand version of the description will be returned -- for example, "M2" instead of "Major second", etc.
	 */
	[[nodiscard]] String getStringDescription (bool useShort = false) const;

	/** Returns the number of cents this interval represents in equal temperament. */
	[[nodiscard]] constexpr int getCents_EqualTemperament() const noexcept;

	/** Returns an array of possible cents values this interval can represent in just intonation, taking enharmonic spelling into account.
	 */
	[[nodiscard]] juce::Array<double> getCents_JustIntonation() const;

	/** Returns true if the quality is valid for the given kind of interval.
		For example, a perfect fourth or fifth is valid, while a perfect third or perfect second is not. A major or minor third is valid, while a major fourth or minor fifth is not.
		Note the special case that a diminished unison is invalid; unisons can only be perfect or augmented.
	 */
	[[nodiscard]] static constexpr bool isValidQualityForKind (Quality quality, int kind) noexcept;

private:

	[[nodiscard]] constexpr bool intervalIsPerfectKind() noexcept;

	Quality quality { Quality::Major };

	int kind { 0 };
};


/** Adds an interval to the given pitch and returns a new pitch object at the resulting pitch. */
Pitch operator+ (const Pitch& pitch, const Interval& interval) noexcept;

/** Subtracts an interval from the given pitch and returns a new pitch object at the resulting pitch. */
Pitch operator- (const Pitch& pitch, const Interval& interval) noexcept;

}  // namespace lemons::music
