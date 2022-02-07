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

class Octatonic final : public Scale
{
public:

	constexpr explicit Octatonic (int pitchClassToStartOn, bool startWithSemitone) noexcept
		: startWithHalfStep (startWithSemitone), pitchClassOfRoot (makeValidPitchClass (pitchClassToStartOn))
	{
	}

	constexpr Octatonic (const Octatonic& other) noexcept
		: startWithHalfStep (other.startWithHalfStep), pitchClassOfRoot (other.pitchClassOfRoot)
	{
	}

	[[nodiscard]] static Octatonic fromStringDescription (const String& string);

	[[nodiscard]] bool operator== (const Octatonic& other) const;
	[[nodiscard]] bool operator!= (const Octatonic& other) const;

	[[nodiscard]] juce::Array<int> getIntervalsAsSemitones() const final;

	[[nodiscard]] int getPitchClassOfRoot() const noexcept final;

	[[nodiscard]] String getStringDescription() const final;

	[[nodiscard]] int notesPerOctave() const noexcept final;

private:

	bool startWithHalfStep { true };

	int pitchClassOfRoot { 0 };
};

}  // namespace lemons::music::scales
