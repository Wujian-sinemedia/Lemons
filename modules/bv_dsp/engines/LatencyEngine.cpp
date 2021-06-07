
namespace bav::dsp
{
template < typename SampleType >
void LatencyEngine< SampleType >::prepared (int blocksize, double samplerate)
{
    inputStorage.setSize (2, blocksize, true, true, true);
    midiChoppingBuffer.ensureSize (static_cast< size_t > (blocksize));
    chunkMidiBuffer.ensureSize (static_cast< size_t > (blocksize));
    inputFIFO.changeSize (2, blocksize);
    outputFIFO.changeSize (2, blocksize);
    inBuffer.setSize (2, blocksize, true, true, true);
    outBuffer.setSize (2, blocksize, true, true, true);
    onPrepare (blocksize, samplerate);
}

template < typename SampleType >
void LatencyEngine< SampleType >::released()
{
    inputStorage.setSize (0, 0);
    midiChoppingBuffer.clear();
    chunkMidiBuffer.clear();
    inputFIFO.clear();
    outputFIFO.clear();
    inBuffer.setSize (0, 0);
    outBuffer.setSize (0, 0);
    internalBlocksize = 0;
    onRelease();
}

template < typename SampleType >
void LatencyEngine< SampleType >::changeLatency (int newInternalBlocksize)
{
    using Engine = Engine< SampleType >;

    internalBlocksize = newInternalBlocksize;
    Engine::prepare (Engine::getSamplerate(), internalBlocksize);
}

template < typename SampleType >
void LatencyEngine< SampleType >::renderBlock (const AudioBuffer& input, AudioBuffer& output, MidiBuffer& midiMessages, bool isBypassed)
{
    jassert (internalBlocksize > 0);

    const auto totalNumSamples = input.getNumSamples();

    if (totalNumSamples == 0)
    {
        renderChunk (input, output, midiMessages, isBypassed);
        return;
    }

    jassert (totalNumSamples == output.getNumSamples());

    const auto numInChannels  = std::min (2, input.getNumChannels());
    const auto numOutChannels = std::min (2, output.getNumChannels());

    if (numInChannels == 0 || numOutChannels == 0)
    {
        renderChunk (input, output, midiMessages, isBypassed);
        return;
    }

    if (totalNumSamples <= internalBlocksize)
    {
        processWrapped (input, output, midiMessages, isBypassed);
        return;
    }

    for (int chan = 0; chan < numInChannels; ++chan)
        inputStorage.copyFrom (chan, 0, input, chan, 0, totalNumSamples);

    int samplesLeft = totalNumSamples;
    int startSample = 0;

    do
    {
        const auto chunkNumSamples = std::min (internalBlocksize, samplesLeft);

        AudioBuffer inputProxy (inputStorage.getArrayOfWritePointers(),
                                numInChannels,
                                startSample,
                                chunkNumSamples);
        AudioBuffer outputProxy (output.getArrayOfWritePointers(),
                                 numOutChannels,
                                 startSample,
                                 chunkNumSamples);

        midiChoppingBuffer.clear();
        bav::midi::copyRangeOfMidiBuffer (midiMessages, midiChoppingBuffer, startSample, 0, chunkNumSamples);

        processWrapped (inputProxy, outputProxy, midiChoppingBuffer, isBypassed);

        bav::midi::copyRangeOfMidiBuffer (midiChoppingBuffer, midiMessages, 0, startSample, chunkNumSamples);

        startSample += chunkNumSamples;
        samplesLeft -= chunkNumSamples;
    } while (samplesLeft > 0);
}

template < typename SampleType >
void LatencyEngine< SampleType >::processWrapped (const AudioBuffer& input, AudioBuffer& output, MidiBuffer& midiMessages, bool isBypassed)
{
    const auto numNewSamples = input.getNumSamples();
    jassert (numNewSamples <= internalBlocksize && numNewSamples > 0);

    inputFIFO.push (input, midiMessages, numNewSamples);

    if (inputFIFO.numStoredSamples() >= internalBlocksize)
    {
        inBuffer.clear();
        chunkMidiBuffer.clear();

        inputFIFO.pop (inBuffer, chunkMidiBuffer, internalBlocksize);

        outBuffer.clear();
        renderChunk (inBuffer, outBuffer, chunkMidiBuffer, isBypassed);

        if (isBypassed)
            outputFIFO.push (inBuffer, chunkMidiBuffer, internalBlocksize);
        else
            outputFIFO.push (outBuffer, chunkMidiBuffer, internalBlocksize);
    }

    outputFIFO.pop (output, midiMessages, numNewSamples);
}

template < typename SampleType >
void LatencyEngine< SampleType >::onPrepare (int, double)
{
}

template class LatencyEngine< float >;
template class LatencyEngine< double >;

}  // namespace bav::dsp
