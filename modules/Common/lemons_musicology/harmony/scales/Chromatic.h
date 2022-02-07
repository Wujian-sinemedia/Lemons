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

class Chromatic final : public Scale
{
public:

	constexpr explicit Chromatic (int pitchClassToStartOn) noexcept
		: startingPitchClass (makeValidPitchClass (pitchClassToStartOn))
	{
	}

	constexpr Chromatic (const Chromatic& other) noexcept
		: startingPitchClass (other.startingPitchClass)
	{
	}

	[[nodiscard]] static Chromatic fromStringDescription (const String& string);

	[[nodiscard]] bool operator== (const Chromatic& other) const;
	[[nodiscard]] bool operator!= (const Chromatic& other) const;

	[[nodiscard]] juce::Array<int> getIntervalsAsSemitones() const final;

	[[nodiscard]] int getPitchClassOfRoot() const noexcept final;

	[[nodiscard]] String getStringDescription() const final;

	[[nodiscard]] int notesPerOctave() const noexcept final;

private:

	const int startingPitchClass { 0 };
};

}  // namespace lemons::music::scales
