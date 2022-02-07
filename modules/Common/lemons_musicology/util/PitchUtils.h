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

#include <array>

namespace lemons::music
{

constexpr bool isValidPitchClass (int pitchClass) noexcept
{
	return pitchClass >= 0 && pitchClass <= 11;
}

constexpr int makeValidPitchClass (int pitchClass) noexcept
{
	return pitchClass % 12;
}

constexpr int octaveNumberOfMidiNote (int midiNote) noexcept
{
	return midiNote / 12 - 1;
}

constexpr int lowestNoteOfMidiOctave (int octaveNumber) noexcept
{
	return (octaveNumber + 1) * 12;
}

constexpr std::array<int, 7> getSharpsOrder()
{
	return { 5, 0, 7, 2, 9, 4, 11 };
}

constexpr std::array<int, 7> getFlatsOrder()
{
	return { 11, 4, 9, 2, 7, 0, 5 };
}

}  // namespace lemons::music
