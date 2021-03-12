
#include "bv_SharedCode/bv_SharedCode.h"


namespace bav
{

namespace dsp
{

    
template<typename SampleType>
FIFOWrappedEngine<SampleType>::FIFOWrappedEngine (int consistentInternalBlocksize, double samplerate): internalBlocksize(0)
{
    initialize (samplerate, consistentInternalBlocksize);
}
    

template<typename SampleType>
FIFOWrappedEngine<SampleType>::FIFOWrappedEngine(): internalBlocksize(0)
{
    
}

    
template<typename SampleType>
FIFOWrappedEngine<SampleType>::~FIFOWrappedEngine() { }
    
    
template<typename SampleType>
void FIFOWrappedEngine<SampleType>::initialize (double samplerate, int newInternalBlocksize)
{
    jassert (samplerate > 0);
    jassert (newInternalBlocksize > 0);
    
    wasBypassedLastCallback = true;
    isInitialized = true;
    resourcesReleased = false;
    
    initialized (newInternalBlocksize, samplerate);
    
    changeLatency (newInternalBlocksize);
}


template<typename SampleType>
void FIFOWrappedEngine<SampleType>::prepare (double samplerate)
{
    jassert (samplerate > 0);
    jassert (isInitialized);
    
    resourcesReleased = false;
    
    prepareToPlay (samplerate);
}
    

template<typename SampleType>
void FIFOWrappedEngine<SampleType>::reset()
{
    midiChoppingBuffer.clear();
    chunkMidiBuffer.clear();
    inBuffer.clear();
    outBuffer.clear();
    inputBuffer.clear();
    outputBuffer.clear();
    midiInputCollection.clear();
    midiOutputCollection.clear();
    
    resetTriggered();
}

    
template<typename SampleType>
void FIFOWrappedEngine<SampleType>::releaseResources()
{
    inBuffer.setSize(0, 0, false, false, false);
    outBuffer.setSize(0, 0, false, false, false);
    
    inputBuffer.releaseResources();
    outputBuffer.releaseResources();
    
    midiChoppingBuffer.clear();
    midiInputCollection.clear();
    midiOutputCollection.clear();
    chunkMidiBuffer.clear();
    
    wasBypassedLastCallback = true;
    resourcesReleased = true;
    isInitialized = false;
    
    release();
}
    

template<typename SampleType>
void FIFOWrappedEngine<SampleType>::changeLatency (int newInternalBlocksize)
{
    jassert (newInternalBlocksize > 0);
    
    internalBlocksize = newInternalBlocksize;
    
    const int doubleBlocksize = internalBlocksize * 2;
    
    inBuffer.setSize (2, doubleBlocksize, true, true, true);
    outBuffer.setSize (2, doubleBlocksize, true, true, true);
    inputBuffer.changeSize (2, doubleBlocksize);
    outputBuffer.changeSize(2, doubleBlocksize);
    
    const size_t doubleBlocksizeT = size_t(doubleBlocksize);
    
    midiInputCollection.setSize(doubleBlocksize);
    midiOutputCollection.setSize(doubleBlocksize);
    chunkMidiBuffer.ensureSize (doubleBlocksizeT);
    midiChoppingBuffer.ensureSize (doubleBlocksizeT);
    
    latencyChanged (newInternalBlocksize);
}
    

template<typename SampleType>
void FIFOWrappedEngine<SampleType>::process (AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output,
                                             MidiBuffer& midiMessages,
                                             const bool isBypassed)
{
    jassert (! resourcesReleased);
    jassert (isInitialized);
    
    const int totalNumSamples = input.getNumSamples();
    
    if (totalNumSamples == 0 || input.getNumChannels() == 0 || output.getNumSamples() == 0 || output.getNumChannels() == 0)
        return;
    
    bool applyFadeIn, applyFadeOut;
    bool processingBypassedThisFrame = isBypassed;
    
    if (isBypassed)
    {
        processingBypassedThisFrame = wasBypassedLastCallback;
        applyFadeIn = false;
        applyFadeOut = ! wasBypassedLastCallback;
    }
    else
    {
        applyFadeOut = false;
        applyFadeIn = wasBypassedLastCallback;
    }
    
    wasBypassedLastCallback = isBypassed;
    
    if (totalNumSamples <= internalBlocksize)
    {
        processWrapped (input, output, midiMessages, applyFadeIn, applyFadeOut, processingBypassedThisFrame);
        return;
    }
    
    int samplesLeft = totalNumSamples;
    int startSample = 0;
    
    bool actuallyFadingIn  = applyFadeIn;
    bool actuallyFadingOut = applyFadeOut;
    
    do {
        const int chunkNumSamples = std::min (internalBlocksize, samplesLeft);
        
        AudioBuffer<SampleType> inputProxy  (input.getArrayOfWritePointers(),  2, startSample, chunkNumSamples);
        AudioBuffer<SampleType> outputProxy (output.getArrayOfWritePointers(), 2, startSample, chunkNumSamples);
        
        /* put just the midi messages for this time segment into the midiChoppingBuffer
         in the midiChoppingBuffer, events will start at timestamp sample 0
         the midi output will be returned by being copied to this same region of the midiChoppingBuffer */
        midiChoppingBuffer.clear();
        bav::midi::copyRangeOfMidiBuffer (midiMessages, midiChoppingBuffer, startSample, 0, chunkNumSamples);
        
        processWrapped (inputProxy, outputProxy, midiChoppingBuffer, actuallyFadingIn, actuallyFadingOut, processingBypassedThisFrame);
        
        // copy the midi output back to midiMessages (I/O), at the original startSample
        bav::midi::copyRangeOfMidiBuffer (midiChoppingBuffer, midiMessages, 0, startSample, chunkNumSamples);
        
        startSample += chunkNumSamples;
        samplesLeft -= chunkNumSamples;
        
        actuallyFadingIn  = false;
        actuallyFadingOut = false;
        processingBypassedThisFrame = isBypassed;
    } while (samplesLeft > 0);
}

template<typename SampleType>
void FIFOWrappedEngine<SampleType>::processWrapped (AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output,
                                                    MidiBuffer& midiMessages,
                                                    const bool applyFadeIn, const bool applyFadeOut,
                                                    const bool isBypassed)
{
    const int numNewSamples = input.getNumSamples();
    
    jassert (numNewSamples <= internalBlocksize);
    
    for (int chan = 0; chan < 2; ++chan)
        inputBuffer.pushSamples (input, chan, 0, numNewSamples, chan);
    
    midiInputCollection.pushEvents (midiMessages, numNewSamples);
    
    if (inputBuffer.numStoredSamples(0) >= internalBlocksize)  // we have enough samples, render the new chunk
    {
        inBuffer.clear();
        for (int chan = 0; chan < 2; ++chan)
            inputBuffer.popSamples (inBuffer, chan, 0, internalBlocksize, chan);
        
        chunkMidiBuffer.clear();
        midiInputCollection.popEvents (chunkMidiBuffer,  internalBlocksize);
        
        AudioBuffer<SampleType> inProxy = AudioBuffer<SampleType> (inBuffer.getArrayOfWritePointers(), 2, 0, internalBlocksize);
        
        if (isBypassed)
        {
            for (int chan = 0; chan < 2; ++chan)
                outputBuffer.pushSamples (inProxy, chan, 0, internalBlocksize, chan);
        }
        else
        {
            outBuffer.clear();
            AudioBuffer<SampleType> outProxy = AudioBuffer<SampleType> (outBuffer.getArrayOfWritePointers(), 2, 0, internalBlocksize);
            
            renderBlock (inProxy, outProxy, chunkMidiBuffer);
            
            for (int chan = 0; chan < 2; ++chan)
                outputBuffer.pushSamples (outProxy, chan, 0, internalBlocksize, chan);
        }
        
        midiOutputCollection.pushEvents (chunkMidiBuffer, internalBlocksize);
    }
    
    for (int chan = 0; chan < 2; ++chan)
        outputBuffer.popSamples (output, chan, 0, numNewSamples, chan);
    
    midiOutputCollection.popEvents (midiMessages, numNewSamples);
    
    if (applyFadeIn)
        output.applyGainRamp (0, numNewSamples, 0.0f, 1.0f);
    
    if (applyFadeOut)
        output.applyGainRamp (0, numNewSamples, 1.0f, 0.0f);
}



template class FIFOWrappedEngine<float>;
template class FIFOWrappedEngine<double>;


}  // namespace dsp

}  // namespace bav


