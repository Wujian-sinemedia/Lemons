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

constexpr bool isValidPitchClass (int pitchClass)
{
	return pitchClass >= 0 && pitchClass <= 11;
}

constexpr int makeValidPitchClass (int pitchClass)
{
	return pitchClass % 12;
}

constexpr int octaveNumberOfMidiNote (int midiNote)
{
	return midiNote / 12 - 1;
}

constexpr int lowestNoteOfMidiOctave (int octaveNumber)
{
	return (octaveNumber + 1) * 12;
}

}  // namespace lemons::music
