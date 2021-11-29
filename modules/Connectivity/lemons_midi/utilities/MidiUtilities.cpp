
namespace lemons::midi
{


void copyRangeOfMidiBuffer (const MidiBuffer& readingBuffer,
                            MidiBuffer&       destBuffer,
                            const int         startSampleOfInput,
                            const int         startSampleOfOutput,
                            const int         numSamples)
{
	if (numSamples == 0) return;

	destBuffer.clear (startSampleOfOutput, numSamples);
	destBuffer.addEvents (readingBuffer,
	                      startSampleOfInput,
	                      numSamples,
	                      startSampleOfOutput - startSampleOfInput);
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

}  // namespace lemons::midi
