
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

}  // namespace lemons::midi
