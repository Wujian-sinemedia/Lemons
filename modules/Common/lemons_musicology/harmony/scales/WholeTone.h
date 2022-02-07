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

class WholeTone final : public Scale
{
public:

	constexpr explicit WholeTone (int pitchClassOfRoot) noexcept
		: startingPitchClass (makeValidPitchClass (pitchClassOfRoot))
	{
	}

	constexpr WholeTone (const WholeTone& other) noexcept
		: startingPitchClass (other.startingPitchClass)
	{
	}

	[[nodiscard]] static WholeTone fromStringDescription (const String& string);

	[[nodiscard]] bool operator== (const WholeTone& other) const;
	[[nodiscard]] bool operator!= (const WholeTone& other) const;

	[[nodiscard]] juce::Array<int> getIntervalsAsSemitones() const final;

	[[nodiscard]] int getNumSharps() const noexcept final;

	[[nodiscard]] int getNumFlats() const noexcept final;

	[[nodiscard]] int getPitchClassOfRoot() const noexcept final;

	[[nodiscard]] String getStringDescription() const final;

	[[nodiscard]] int notesPerOctave() const noexcept final;

private:

	const int startingPitchClass { 0 };
};

}  // namespace lemons::music::scales
