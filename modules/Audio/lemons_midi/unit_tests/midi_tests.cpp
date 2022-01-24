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


namespace lemons::tests
{

MidiTest::MidiTest (const String& testName)
	: Test (testName, "MIDI")
{
}

void fillMidiBufferWithRandomEvents (MidiBuffer& buffer, int numEvents, juce::Random rng)
{
	buffer.clear();

	for (int i = 0; i < numEvents; ++i)
		buffer.addEvent (juce::MidiMessage::controllerEvent (1, rng.nextInt (128), rng.nextInt (128)),
						 i);
}

bool midiBuffersAreEqual (const MidiBuffer& buffer1,
						  const MidiBuffer& buffer2)
{
	if (buffer1.getNumEvents() != buffer2.getNumEvents())
		return false;

	if (buffer1.getFirstEventTime() != buffer2.getFirstEventTime())
		return false;

	if (buffer1.getLastEventTime() != buffer2.getLastEventTime())
		return false;

	for (auto it1 = buffer1.begin(), it2 = buffer2.begin();
		 it1 != buffer1.end() && it2 != buffer2.end();
		 ++it1, ++it2)
	{
		auto meta1 = *it1;
		auto meta2 = *it2;

		if (meta1.samplePosition != meta2.samplePosition)
			return false;

		if (meta1.getMessage().getDescription() != meta2.getMessage().getDescription())
			return false;
	}

	return true;
}

MidiBuffer makeCopyOfMidiBuffer (const MidiBuffer& inputBuffer)
{
	MidiBuffer newBuffer;

	if (const auto numSamples = inputBuffer.getLastEventTime();
		numSamples > 0)
	{
		newBuffer.addEvents (inputBuffer, 0, numSamples, 0);
		jassert (! newBuffer.isEmpty());
	}

	return newBuffer;
}

}  // namespace lemons::tests
