
namespace lemons::midi
{


void copyRangeOfMidiBuffer (const MidiBuffer& readingBuffer,
                            MidiBuffer&       destBuffer,
                            int               startSampleOfInput,
                            int               startSampleOfOutput,
                            int               numSamples)
{
	if (numSamples == 0) return;

	destBuffer.clear (startSampleOfOutput, numSamples);
	destBuffer.addEvents (readingBuffer,
	                      startSampleOfInput,
	                      numSamples,
	                      startSampleOfOutput - startSampleOfInput);
}


ScopedMidiBufferAlias::ScopedMidiBufferAlias (MidiBuffer& originalBuffer,
                                              MidiBuffer& aliasBufferToUse,
                                              int         startSampleInOrigBuffer,
                                              int         numSamplesToCopy)
    : origBuffer (originalBuffer)
    , aliasBuffer (aliasBufferToUse)
    , startSample (startSampleInOrigBuffer)
    , numSamples (numSamplesToCopy)
{
	aliasBuffer.clear();
	copyRangeOfMidiBuffer (origBuffer, aliasBuffer, startSample, 0, numSamples);
}

ScopedMidiBufferAlias::~ScopedMidiBufferAlias()
{
	copyRangeOfMidiBuffer (aliasBuffer, origBuffer, 0, startSample, numSamples);
}

}  // namespace lemons::midi
