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

class Pitch final
{
public:

	template <typename T>
	constexpr explicit Pitch (T midiNote) noexcept
		: midiPitch (static_cast<double> (midiNote))
	{
	}

	explicit Pitch (const String& pitchString) noexcept;

	constexpr Pitch (const Pitch& other) noexcept;

	[[nodiscard]] constexpr bool operator== (const Pitch& other) const noexcept;
	[[nodiscard]] constexpr bool operator!= (const Pitch& other) const noexcept;

	[[nodiscard]] bool approximatelyEqual (const Pitch& other) const noexcept;

	[[nodiscard]] constexpr bool operator> (const Pitch& other) const noexcept;
	[[nodiscard]] constexpr bool operator< (const Pitch& other) const noexcept;

	[[nodiscard]] double getFreqHz() const noexcept;

	[[nodiscard]] int getRoundedFreqHz() const noexcept;

	[[nodiscard]] constexpr double getMidiPitch() const noexcept;

	[[nodiscard]] int getRoundedMidiPitch() const noexcept;

	[[nodiscard]] int getPitchClass() const noexcept;

	[[nodiscard]] int getOctaveNumber() const noexcept;

	[[nodiscard]] bool isBlackKey() const noexcept;

	[[nodiscard]] bool isWhiteKey() const noexcept;

	[[nodiscard]] String toString() const noexcept;

private:

	double midiPitch { 60. };
};

}  // namespace lemons::music
