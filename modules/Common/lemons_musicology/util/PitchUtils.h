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
	jassert (octaveNumber >= 0);
	return (octaveNumber + 1) * 12;
}

/** Returns an array of pitch class objects representing each sharp in the order they appear in key signatures -- F, C, G, D, A, E, B.
 */
constexpr std::array<PitchClass, 7> getSharpsOrder()
{
	return { PitchClass { 6 }, PitchClass { 1 }, PitchClass { 8 }, PitchClass { 3 }, PitchClass { 10 }, PitchClass { 5 }, PitchClass { 12 } };
}

/** Returns an array of pitch class objects representing each flat in the order they appear in key signatures -- B, E, A, D, G, C, F.
 */
constexpr std::array<PitchClass, 7> getFlatsOrder()
{
	return { PitchClass { 10 }, PitchClass { 3 }, PitchClass { 8 }, PitchClass { 1 }, PitchClass { 6 }, PitchClass { 11 }, PitchClass { 4 } };
}

}  // namespace lemons::music
