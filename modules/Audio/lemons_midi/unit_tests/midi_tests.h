
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

#include <juce_audio_basics/juce_audio_basics.h>
#include <lemons_core/lemons_core.h>

namespace lemons::tests
{

/** @defgroup lemons_midi_unit_tests MIDI tests
	@ingroup lemons_unit_testing lemons_midi
	Utilities for unit testing MIDI algorithms.
 */

using MidiBuffer = juce::MidiBuffer;

/** @ingroup lemons_midi_unit_tests
 */
struct MidiTest : public Test
{
	explicit MidiTest (const String& testName);
};

/** @ingroup lemons_midi_unit_tests
 */
void fillMidiBufferWithRandomEvents (MidiBuffer& buffer, int numEvents, juce::Random rng);

/** @ingroup lemons_midi_unit_tests
 */
[[nodiscard]] bool midiBuffersAreEqual (const MidiBuffer& buffer1, const MidiBuffer& buffer2);

/** @ingroup lemons_midi_unit_tests
 */
[[nodiscard]] MidiBuffer makeCopyOfMidiBuffer (const MidiBuffer& inputBuffer);

}  // namespace lemons::tests
