/*
 ======================================================================================

 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.

 ======================================================================================
 */


#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <lemons_core/lemons_core.h>

namespace lemons::tests
{

using MidiBuffer = juce::MidiBuffer;

struct MidiTest : public Test
{
	MidiTest (const String& testName);
};

void fillMidiBufferWithRandomEvents (MidiBuffer& buffer, int numEvents, juce::Random rng);

[[nodiscard]] bool midiBuffersAreEqual (const MidiBuffer& buffer1, const MidiBuffer& buffer2);

[[nodiscard]] MidiBuffer makeCopyOfMidiBuffer (const MidiBuffer& inputBuffer);

}  // namespace lemons::tests
