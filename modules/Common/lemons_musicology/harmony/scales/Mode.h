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

class Mode final : public Scale
{
public:

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

	constexpr explicit Mode (Type typeToUse, int pitchClassOfRoot) noexcept
		: type (typeToUse), rootPitchClass (makeValidPitchClass (pitchClassOfRoot))
	{
	}

	constexpr Mode (const Mode& other) noexcept
		: type (other.type), rootPitchClass (other.rootPitchClass)
	{
	}

	[[nodiscard]] static Mode fromStringDescription (const String& description);

	[[nodiscard]] constexpr bool operator== (const Mode& other) const noexcept
	{
		return type == other.type && rootPitchClass == other.rootPitchClass;
	}

	[[nodiscard]] constexpr bool operator!= (const Mode& other) const noexcept
	{
		return ! (*this == other);
	}

	[[nodiscard]] juce::Array<int> getIntervalsAsSemitones() const final;

	[[nodiscard]] int getNumSharps() const noexcept final;

	[[nodiscard]] int getNumFlats() const noexcept final;

	[[nodiscard]] int getPitchClassOfRoot() const noexcept final;

	[[nodiscard]] String getStringDescription() const final;

	[[nodiscard]] int notesPerOctave() const noexcept final;

private:

	const Type type { Type::Ionian };

	const int rootPitchClass { 0 };
};

}  // namespace lemons::music::scales
