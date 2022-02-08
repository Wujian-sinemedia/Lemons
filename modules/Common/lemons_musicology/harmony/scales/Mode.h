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

/** Represents a modal scale.
 */
class Mode final : public Scale
{
public:

	/** Represents which kind of mode this scale is. */
	enum class Type
	{
		Ionian,
		Dorian,
		Phrygian,
		Lydian,
		Mixolydian,
		Aeolian,
		Locrian
	};

	/** Creates a modal scale object with the given type and root. */
	constexpr explicit Mode (Type typeToUse, int pitchClassOfRoot) noexcept
		: type (typeToUse), rootPitchClass (makeValidPitchClass (pitchClassOfRoot))
	{
	}

	/** Copy constructor. */
	constexpr Mode (const Mode& other) noexcept
		: type (other.type), rootPitchClass (other.rootPitchClass)
	{
	}

	/** Creates a modal scale object from a string description of one.
		@see getStringDescription()
	 */
	[[nodiscard]] static Mode fromStringDescription (const String& description);

	/** Returns true if the other modal scale has the same type and root as this one. */
	[[nodiscard]] constexpr bool operator== (const Mode& other) const noexcept
	{
		return type == other.type && rootPitchClass == other.rootPitchClass;
	}

	/** Returns true if the other modal scale does not have the same type and root as this one. */
	[[nodiscard]] constexpr bool operator!= (const Mode& other) const noexcept
	{
		return ! (*this == other);
	}

	/** Returns the set of intervals that make up this mode. */
	[[nodiscard]] juce::Array<int> getIntervalsAsSemitones() const final;

	/** Returns the pitch class of the root of this mode. */
	[[nodiscard]] int getPitchClassOfRoot() const noexcept final;

	/** Returns a string description of this mode, for example, "E lydian". */
	[[nodiscard]] String getStringDescription() const final;

	/** Returns 8. */
	[[nodiscard]] int notesPerOctave() const noexcept final;

	/** Returns the number of sharps that make up the key signature for this mode. */
	[[nodiscard]] int getNumSharps() const noexcept;

	/** Returns the number of flats that make up the key signature for this mode. */
	[[nodiscard]] int getNumFlats() const noexcept;

private:

	const Type type { Type::Ionian };

	const int rootPitchClass { 0 };
};

}  // namespace lemons::music::scales
