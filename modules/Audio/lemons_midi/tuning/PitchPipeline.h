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
 *  ======================================================================================
 */

#pragma once

namespace lemons::midi
{

/** @ingroup lemons_midi_tuning
 */
struct PitchPipeline
{
	float getFrequencyForMidi (int midiPitch, int midiChannel = -1) const;
	float getFrequencyForMidi (float midiPitch, int midiChannel = -1) const;

	float getMidiForFrequency (int midiPitch, int midiChannel = -1) const;
	float getMidiForFrequency (float midiPitch, int midiChannel = -1) const;

	void reset();

	PitchBendTracker bend;
	MtsClient		 tuning;
};

}  // namespace lemons::midi
