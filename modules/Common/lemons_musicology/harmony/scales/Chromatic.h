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

namespace lemons::music::scales
{

/** A class representing a chromatic scale.
	Chromatic scale objects only differ from one another by their starting pitch.
 */
class Chromatic final : public Scale
{
public:

	/** Creates a chromatic scale that starts on the specified pitch class. */
	constexpr explicit Chromatic (int pitchClassToStartOn) noexcept
		: startingPitchClass (makeValidPitchClass (pitchClassToStartOn))
	{
	}

	/** Copy constructor. */
	constexpr Chromatic (const Chromatic& other) noexcept
		: startingPitchClass (other.startingPitchClass)
	{
	}

	/** Creates a chromatic scale object from a string description of one.
		@see getStringDescription()
	 */
	[[nodiscard]] static Chromatic fromStringDescription (const String& string);

	/** Returns true if the other chromatic scale starts on the same pitch class as this one. */
	[[nodiscard]] bool operator== (const Chromatic& other) const;

	/** Returns true if the other chromatic scale does not start on the same pitch class as this one. */
	[[nodiscard]] bool operator!= (const Chromatic& other) const;

	/** Returns an array of 12 1's, since every interval in a chromatic scale is a half step. */
	[[nodiscard]] juce::Array<int> getIntervalsAsSemitones() const final;

	/** Returns the pitch class that this chromatic scale starts on. */
	[[nodiscard]] int getPitchClassOfRoot() const noexcept final;

	/** Returns a string description of this scale, for example, "C chromatic". */
	[[nodiscard]] String getStringDescription() const final;

	/** Returns 12. */
	[[nodiscard]] int notesPerOctave() const noexcept final;

private:

	const int startingPitchClass { 0 };
};

}  // namespace lemons::music::scales
