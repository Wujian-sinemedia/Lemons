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

/** Returns true if the passed integer is a valid representation of a pitch class. Valid pitch classes are between 0 and 11 (inclusive).
 */
constexpr bool isValidPitchClass (int pitchClass) noexcept
{
	return pitchClass >= 0 && pitchClass <= 11;
}

/** Creates a valid pitch class integer from the passed integer using the modulus operator.
 */
constexpr int makeValidPitchClass (int pitchClass) noexcept
{
	return pitchClass % 12;
}

/** Returns the octave number of a given MIDI pitch.
 */
constexpr int octaveNumberOfMidiNote (int midiNote) noexcept
{
	return midiNote / 12 - 1;
}

/** Returns the lowest note of a given MIDI octave number (that octave's C key).
 */
constexpr int lowestNoteOfMidiOctave (int octaveNumber) noexcept
{
	return (octaveNumber + 1) * 12;
}

/** Returns a list of pitch class integers representing the order of the sharps -- F, C, G, D, A, E, B.
 */
constexpr std::array<int, 7> getSharpsOrder()
{
	return { 5, 0, 7, 2, 9, 4, 11 };
}

/** Returns a list of pitch class integers representing the order of the flats -- B, E, A, D, G, C, F.
 */
constexpr std::array<int, 7> getFlatsOrder()
{
	return { 11, 4, 9, 2, 7, 0, 5 };
}

}  // namespace lemons::music
