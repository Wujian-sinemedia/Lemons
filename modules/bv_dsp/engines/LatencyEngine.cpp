
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
    
    if (internalBlocksize == newInternalBlocksize) return;

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

    if (input.getNumChannels() == 0 || output.getNumChannels() == 0)
    {
        renderChunk (input, output, midiMessages, isBypassed);
        return;
    }

    if (totalNumSamples <= internalBlocksize)
    {
        processInternal (input, output, midiMessages, isBypassed);
        return;
    }

    processInChunks (input, output, midiMessages, isBypassed);
}

template < typename SampleType >
void LatencyEngine< SampleType >::processInChunks (const AudioBuffer& input, AudioBuffer& output, MidiBuffer& midiMessages, bool isBypassed)
{
    const auto numInChannels  = input.getNumChannels();
    const auto numOutChannels = output.getNumChannels();

    int samplesLeft = input.getNumSamples();

    for (int chan = 0; chan < numInChannels; ++chan)
        inputStorage.copyFrom (chan, 0, input, chan, 0, samplesLeft);

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
        midi::copyRangeOfMidiBuffer (midiMessages, midiChoppingBuffer, startSample, 0, chunkNumSamples);

        processInternal (inputProxy, outputProxy, midiChoppingBuffer, isBypassed);

        midi::copyRangeOfMidiBuffer (midiChoppingBuffer, midiMessages, 0, startSample, chunkNumSamples);

        startSample += chunkNumSamples;
        samplesLeft -= chunkNumSamples;
    } while (samplesLeft > 0);
}

template < typename SampleType >
void LatencyEngine< SampleType >::processInternal (const AudioBuffer& input, AudioBuffer& output, MidiBuffer& midiMessages, bool isBypassed)
{
    const auto numNewSamples = input.getNumSamples();
    
    if (numNewSamples == 0)
    {
        chunkMidiBuffer.clear();
        renderChunk (inBuffer, outBuffer, chunkMidiBuffer, isBypassed);
        return;
    }
    
    jassert (numNewSamples <= internalBlocksize && numNewSamples > 0);

    inputFIFO.push (input, midiMessages, numNewSamples);

    if (inputFIFO.numStoredSamples() >= internalBlocksize)
    {
        inBuffer.clear();
        outBuffer.clear();
        chunkMidiBuffer.clear();

        inputFIFO.pop (inBuffer, chunkMidiBuffer, internalBlocksize);

        renderChunk (inBuffer, outBuffer, chunkMidiBuffer, isBypassed);

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
