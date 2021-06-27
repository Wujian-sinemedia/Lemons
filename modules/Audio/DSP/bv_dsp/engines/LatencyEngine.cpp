
namespace bav::dsp
{
template < typename SampleType >
void LatencyEngine< SampleType >::prepared (int blocksize, double samplerate)
{
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
    
    jassert (newInternalBlocksize > 0);
    
    internalBlocksize = newInternalBlocksize;
    Engine::prepare (Engine::getSamplerate(), internalBlocksize);
}

template < typename SampleType >
void LatencyEngine< SampleType >::renderBlock (const AudioBuffer& input, AudioBuffer& output, MidiBuffer& midiMessages, bool isBypassed)
{
    jassert (internalBlocksize > 0);

    const auto totalNumSamples = input.getNumSamples();

    if (input.getNumChannels() == 0 || output.getNumChannels() == 0)
    {
        renderChunk (input, output, midiMessages, isBypassed);
        return;
    }
    
    jassert (totalNumSamples == output.getNumSamples());

    if (totalNumSamples == 0)
    {
        chunkMidiBuffer.clear();
        renderChunk (inBuffer, outBuffer, chunkMidiBuffer, isBypassed);
        return;
    }
    
    inputFIFO.push (input, midiMessages, totalNumSamples);
    
    while (inputFIFO.numStoredSamples() >= internalBlocksize)
    {
        inBuffer.clear();
        outBuffer.clear();
        chunkMidiBuffer.clear();
        
        inputFIFO.pop (inBuffer, chunkMidiBuffer, internalBlocksize);
        
        renderChunk (inBuffer, outBuffer, chunkMidiBuffer, isBypassed);
        
        outputFIFO.push (outBuffer, chunkMidiBuffer, internalBlocksize);
    }
    
    outputFIFO.pop (output, midiMessages, totalNumSamples);
}

template < typename SampleType >
void LatencyEngine< SampleType >::onPrepare (int, double)
{
}

template class LatencyEngine< float >;
template class LatencyEngine< double >;

}  // namespace bav::dsp
