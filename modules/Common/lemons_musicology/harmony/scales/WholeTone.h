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

/** A class that represents a whole tone scale.
	Whole tone scale objects only differ from one another by their starting pitch.
 */
class WholeTone final : public Scale
{
public:

	/** Creates a whole tone scale object with the given pitch class as its root. */
	constexpr explicit WholeTone (int pitchClassOfRoot) noexcept
		: startingPitchClass (makeValidPitchClass (pitchClassOfRoot))
	{
	}

	/** Copy constructor. */
	constexpr WholeTone (const WholeTone& other) noexcept
		: startingPitchClass (other.startingPitchClass)
	{
	}

	/** Creates a whole tone scale object from a string description of one.
		@see getStringDescription()
	 */
	[[nodiscard]] static WholeTone fromStringDescription (const String& string);

	/** Returns true if the other whole tone scale has the same root as this one. */
	[[nodiscard]] bool operator== (const WholeTone& other) const;

	/** Returns true if the other whole tone scale does not have the same root as this one. */
	[[nodiscard]] bool operator!= (const WholeTone& other) const;

	/** Returns an array of 6 2's, since every interval in this scale is a whole step. */
	[[nodiscard]] juce::Array<int> getIntervalsAsSemitones() const final;

	/** Returns the pitch class of the root of this scale. */
	[[nodiscard]] int getPitchClassOfRoot() const noexcept final;

	/** Returns a string description of this scale, for example, "C# whole tone".
		@see fromStringDescription()
	 */
	[[nodiscard]] String getStringDescription() const final;

	/** Returns 6. */
	[[nodiscard]] int notesPerOctave() const noexcept final;

private:

	const int startingPitchClass { 0 };
};

}  // namespace lemons::music::scales
