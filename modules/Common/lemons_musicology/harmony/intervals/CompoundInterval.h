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
	This class is capable of representing intervals smaller than an octave, but if you know your interval is going to be smaller than an octave, prefer to use the Interval class directly.
 */
class CompoundInterval final
{
public:

	// TO DO: to/from string description

	/** Creates a compound interval with the given number of octaves and the given simple interval component.
		If the simple interval is a perfect or augmented octave, the number of octaves will be incremented and the simple interval component will be reduced to a perfect or augmented unison.
	 */
	constexpr explicit CompoundInterval (int octaves, const Interval& simpleInterval) noexcept;

	/** Creates a compound interval from a given kind and quality, eg, major ninth, etc. */
	constexpr explicit CompoundInterval (int kind, Interval::Quality quality) noexcept;

	/** Constructs a compound interval with the given number of semitones.
	 */
	constexpr explicit CompoundInterval (int numSemitones) noexcept;

	/** Returns a compound interval object representing the distance between two MIDI notes.
	 */
	[[nodiscard]] static constexpr CompoundInterval fromPitches (int midiPitch1, int midiPitch2) noexcept;

	/** Returns a compound interval object representing the distance between two pitch objects. */
	[[nodiscard]] static CompoundInterval fromPitches (const Pitch& pitch1, const Pitch& pitch2) noexcept;

	/** Copy constructor. */
	constexpr CompoundInterval (const CompoundInterval& other) noexcept;

	/** Assignment operator. */
	constexpr CompoundInterval& operator= (const CompoundInterval& other) noexcept;

	/** Returns true if the two compound intervals are exactly equal; that is, have the same number of octaves, and their simple interval components are exactly equal in kind and quality. */
	[[nodiscard]] constexpr bool operator== (const CompoundInterval& other) const noexcept;

	/** Returns true if the two intervals represent the same number of semitones, regardless of their enharmonic spelling. */
	[[nodiscard]] constexpr bool isEnharmonicTo (const CompoundInterval& other) const noexcept;

	/** Returns true if the two intervals are not exactly equal. */
	[[nodiscard]] constexpr bool operator!= (const CompoundInterval& other) const noexcept;

	/** Returns true if this interval is semantically larger than the other one, taking enharmonic spellings into account. */
	[[nodiscard]] constexpr bool operator> (const CompoundInterval& other) const noexcept;

	/** Returns true if this interval is semantically smaller than the other one, taking enharmonic spellings into account. */
	[[nodiscard]] constexpr bool operator< (const CompoundInterval& other) const noexcept;

	/** Returns the number of octaves contained in this compound interval. */
	[[nodiscard]] constexpr int getNumOctaves() const noexcept;

	/** Returns the simple interval component of this compound interval. The interval object returned by this function will always be a diminished octave or smaller, and may be a perfect unison. */
	[[nodiscard]] constexpr Interval getSimpleInterval() const noexcept;

	/** Returns the number of semitones this compound interval represents. */
	[[nodiscard]] constexpr int getNumSemitones() const noexcept;

	/** Returns a new pitch object that represents the note this interval's distance above or below the passed pitch object.
		@param other The pitch object to start from.
		@param above When true, the returned pitch will be this interval's distance above the passed pitch. When false, the returned pitch will be this interval's distance below the passed pitch.
	 */
	[[nodiscard]] Pitch applyToPitch (const Pitch& other, bool above) const noexcept;

	/** Returns true if the two compound intervals have the same sub-octave component, regardless of how many octaves either compound interval spans.
		For example, a major tenth and a major seventeenth would return true, because they are both compound major thirds.
	 */
	[[nodiscard]] constexpr bool hasSameSimpleInterval (const CompoundInterval& other) const noexcept;

	/** Returns the quality of this compound interval.
		The quality of compound intervals is determined by the quality of their simple interval component; this is identical to calling:
		@code
		compoundInterval.getSimpleInterval().getQuality();
		@endcode
	 */
	[[nodiscard]] constexpr Interval::Quality getQuality() const noexcept;

	/** Returns the kind of this compound interval as an integer, eg, 9, 10, etc.
	 */
	[[nodiscard]] constexpr int getKind() const noexcept;

	/** Returns a string description of this compound interval.
		@param useShort When true, a shorthand version of the description will be returned -- for example, "M9" instead of "Major ninth", etc.
	 */
	[[nodiscard]] String getStringDescription (bool useShort = false) const;

	/** Returns the number of cents this interval represents in equal temperament. */
	[[nodiscard]] constexpr int getCents_EqualTemperament() const noexcept;

	/** Returns an array of possible cents values this interval can represent in just intonation, taking enharmonic spelling into account.
	 */
	[[nodiscard]] juce::Array<double> getCents_JustIntonation() const;

private:

	Interval interval;

	int numOctaves { 0 };
};


/** Adds a compound interval to the given pitch and returns a new pitch object at the resulting pitch. */
Pitch operator+ (const Pitch& pitch, const CompoundInterval& interval) noexcept;

/** Subtracts a compound interval from the given pitch and returns a new pitch object at the resulting pitch. */
Pitch operator- (const Pitch& pitch, const CompoundInterval& interval) noexcept;

}  // namespace lemons::music
