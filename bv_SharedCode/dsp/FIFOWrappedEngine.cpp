
#include "bv_SharedCode/bv_SharedCode.h"


namespace bav
{

namespace dsp
{
    
    template<typename SampleType>
    FIFOWrappedEngine<SampleType>::FIFOWrappedEngine(int initInternalBlocksize): internalBlocksize(initInternalBlocksize)
    {
        inputBuffer.initialize(2, internalBlocksize * 2);
        outputBuffer.initialize(2, internalBlocksize * 2);
    }
    
    template<typename SampleType>
    FIFOWrappedEngine<SampleType>::~FIFOWrappedEngine() { }
    
    
    template<typename SampleType>
    void FIFOWrappedEngine<SampleType>::prepare (double samplerate, int blocksize)
    {
        jassert (samplerate > 0);
        jassert (blocksize > 0);
        
        inBuffer.setSize (2, internalBlocksize, true, true, true);
        outBuffer.setSize(2, internalBlocksize, true, true, true);
        inputBuffer.changeSize (2, internalBlocksize * 2);
        outputBuffer.changeSize(2, internalBlocksize * 3);
        
        prepareToPlay (samplerate, blocksize);
    }
    
    template<typename SampleType>
    void FIFOWrappedEngine<SampleType>::releaseResources()
    {
        inBuffer.setSize(0, 0, false, false, false);
        outBuffer.setSize(0, 0, false, false, false);
        
        inputBuffer.releaseResources();
        outputBuffer.releaseResources();
        
        release();
    }
    
    template<typename SampleType>
    void FIFOWrappedEngine<SampleType>::changeLatency (int newInternalBlocksize)
    {
        if (internalBlocksize == newInternalBlocksize)
            return;
        
        internalBlocksize = newInternalBlocksize;
        
        inBuffer.setSize (2, internalBlocksize, true, true, true);
        outBuffer.setSize (2, internalBlocksize, true, true, true);
        
        inputBuffer.changeSize (2, internalBlocksize * 2);
        outputBuffer.changeSize(2, internalBlocksize * 3);
        
        latencyChanged (newInternalBlocksize);
    }
    
    template<typename SampleType>
    void FIFOWrappedEngine<SampleType>::process (AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output,
                                                 const bool applyFadeIn, const bool applyFadeOut,
                                                 const bool isBypassed)
    {
        const int totalNumSamples = input.getNumSamples();
        
        if (totalNumSamples == 0 || input.getNumChannels() == 0 || output.getNumSamples() == 0 || output.getNumChannels() == 0)
            return;
        
        if (totalNumSamples <= internalBlocksize)
        {
            processWrapped (input, output, applyFadeIn, applyFadeOut, isBypassed);
            return;
        }
        
        int samplesLeft = totalNumSamples;
        int startSample = 0;
        
        bool actuallyFadingIn  = applyFadeIn;
        bool actuallyFadingOut = applyFadeOut;
        
        do {
            const int chunkNumSamples = std::min (internalBlocksize, samplesLeft);
            
            AudioBuffer<SampleType> inputProxy  (input.getArrayOfWritePointers(),  input.getNumChannels(), startSample, chunkNumSamples);
            AudioBuffer<SampleType> outputProxy (output.getArrayOfWritePointers(), 2,                      startSample, chunkNumSamples);
            
            processWrapped (inputProxy, outputProxy, actuallyFadingIn, actuallyFadingOut, isBypassed);
            
            startSample += chunkNumSamples;
            samplesLeft -= chunkNumSamples;
            
            actuallyFadingIn  = false;
            actuallyFadingOut = false;
        }
        while (samplesLeft > 0);
    }
    
    template<typename SampleType>
    void FIFOWrappedEngine<SampleType>::processWrapped (AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output,
                                                        const bool applyFadeIn, const bool applyFadeOut,
                                                        const bool isBypassed)
    {
        const int numNewSamples = input.getNumSamples();
        
        jassert (numNewSamples <= internalBlocksize);
        
        inputBuffer.pushSamples (input, 0, 0, numNewSamples, 0);
        
        if (inputBuffer.numStoredSamples() >= internalBlocksize)  // we have enough samples, render the new chunk
        {
            inBuffer.clear();
            inputBuffer.popSamples (inBuffer, 0, 0, internalBlocksize, 0);
            
            if (isBypassed)
            {
                for (int chan = 0; chan < 2; ++chan)
                    outputBuffer.pushSamples (inBuffer, 0, 0, internalBlocksize, chan);
            }
            else
            {
                AudioBuffer<SampleType> outProxy = AudioBuffer<SampleType> (outBuffer.getArrayOfWritePointers(),
                                                                            outBuffer.getNumChannels(), 0, internalBlocksize);
                
                renderBlock (inBuffer, outProxy);
                
                for (int chan = 0; chan < outBuffer.getNumChannels(); ++chan)
                    outputBuffer.pushSamples (outBuffer, chan, 0, internalBlocksize, chan);
            }
        }
        
        for (int chan = 0; chan < output.getNumChannels(); ++chan)
            outputBuffer.popSamples (output, chan, 0, numNewSamples, chan);
        
        if (applyFadeIn)
            output.applyGainRamp (0, numNewSamples, 0.0f, 1.0f);
        
        if (applyFadeOut)
            output.applyGainRamp (0, numNewSamples, 1.0f, 0.0f);
    }
    
    
    template class FIFOWrappedEngine<float>;
    template class FIFOWrappedEngine<double>;
    
    
/****************************************************************************************************************************************************
****************************************************************************************************************************************************/
    
    
    
    template<typename SampleType>
    FIFOWrappedEngineWithMidi<SampleType>::FIFOWrappedEngineWithMidi(int initInternalBlocksize): internalBlocksize(initInternalBlocksize)
    {
        inputBuffer.initialize(2, internalBlocksize * 2);
        outputBuffer.initialize(2, internalBlocksize * 2);
    }
    
    template<typename SampleType>
    FIFOWrappedEngineWithMidi<SampleType>::~FIFOWrappedEngineWithMidi() { }
    
    
    template<typename SampleType>
    void FIFOWrappedEngineWithMidi<SampleType>::prepare (double samplerate, int blocksize)
    {
        jassert (samplerate > 0);
        jassert (blocksize > 0);
        
        const size_t doubleBlocksize = size_t(internalBlocksize * 2);
        
        midiInputCollection .ensureSize (doubleBlocksize);
        midiOutputCollection.ensureSize (doubleBlocksize);
        chunkMidiBuffer.ensureSize (doubleBlocksize);
        midiChoppingBuffer.ensureSize (doubleBlocksize);
        
        inBuffer.setSize (2, internalBlocksize, true, true, true);
        outBuffer.setSize(2, internalBlocksize, true, true, true);
        inputBuffer.changeSize (2, internalBlocksize * 2);
        outputBuffer.changeSize(2, internalBlocksize * 3);
        
        prepareToPlay (samplerate, blocksize);
    }
    
    template<typename SampleType>
    void FIFOWrappedEngineWithMidi<SampleType>::releaseResources()
    {
        inBuffer.setSize(0, 0, false, false, false);
        outBuffer.setSize(0, 0, false, false, false);
        
        inputBuffer.releaseResources();
        outputBuffer.releaseResources();
        
        midiChoppingBuffer.clear();
        midiInputCollection.clear();
        midiOutputCollection.clear();
        chunkMidiBuffer.clear();
        
        release();
    }
    
    template<typename SampleType>
    void FIFOWrappedEngineWithMidi<SampleType>::changeLatency (int newInternalBlocksize)
    {
        if (internalBlocksize == newInternalBlocksize)
            return;
        
        internalBlocksize = newInternalBlocksize;
        
        inBuffer.setSize (2, internalBlocksize, true, true, true);
        outBuffer.setSize (2, internalBlocksize, true, true, true);
        
        inputBuffer.changeSize (2, internalBlocksize * 2);
        outputBuffer.changeSize(2, internalBlocksize * 3);
        
        latencyChanged (newInternalBlocksize);
    }
    
    template<typename SampleType>
    void FIFOWrappedEngineWithMidi<SampleType>::process (AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midiMessages,
                                                         const bool applyFadeIn, const bool applyFadeOut,
                                                         const bool isBypassed)
    {
        const int totalNumSamples = input.getNumSamples();
        
        if (totalNumSamples == 0 || input.getNumChannels() == 0 || output.getNumSamples() == 0 || output.getNumChannels() == 0)
            return;
        
        if (totalNumSamples <= internalBlocksize)
        {
            processWrapped (input, output, midiMessages, applyFadeIn, applyFadeOut, isBypassed);
            return;
        }
        
        int samplesLeft = totalNumSamples;
        int startSample = 0;
        
        bool actuallyFadingIn  = applyFadeIn;
        bool actuallyFadingOut = applyFadeOut;
        
        do {
            const int chunkNumSamples = std::min (internalBlocksize, samplesLeft);
            
            AudioBuffer<SampleType> inputProxy  (input.getArrayOfWritePointers(),  input.getNumChannels(), startSample, chunkNumSamples);
            AudioBuffer<SampleType> outputProxy (output.getArrayOfWritePointers(), 2,                      startSample, chunkNumSamples);
            
            /* put just the midi messages for this time segment into the midiChoppingBuffer
             in the midiChoppingBuffer, events will start at timestamp sample 0
             the harmonizer's midi output will be returned by being copied to this same region of the midiChoppingBuffer */
            midiChoppingBuffer.clear();
            bav::midi::copyRangeOfMidiBuffer (midiMessages, midiChoppingBuffer, startSample, 0, chunkNumSamples);
            
            processWrapped (inputProxy, outputProxy, midiChoppingBuffer, actuallyFadingIn, actuallyFadingOut, isBypassed);
            
            // copy the harmonizer's midi output back to midiMessages (I/O), at the original startSample
            bav::midi::copyRangeOfMidiBuffer (midiChoppingBuffer, midiMessages, 0, startSample, chunkNumSamples);
            
            startSample += chunkNumSamples;
            samplesLeft -= chunkNumSamples;
            
            actuallyFadingIn  = false;
            actuallyFadingOut = false;
        }
        while (samplesLeft > 0);
    }
    
    template<typename SampleType>
    void FIFOWrappedEngineWithMidi<SampleType>::processWrapped (AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output,
                                                                MidiBuffer& midiMessages,
                                                                const bool applyFadeIn, const bool applyFadeOut,
                                                                const bool isBypassed)
    {
        const int numNewSamples = input.getNumSamples();
        
        jassert (numNewSamples <= internalBlocksize);
        
        inputBuffer.pushSamples (input, 0, 0, numNewSamples, 0);
        midiInputCollection.pushEvents (midiMessages, numNewSamples);
        
        if (inputBuffer.numStoredSamples() >= internalBlocksize)  // we have enough samples, render the new chunk
        {
            inBuffer.clear();
            inputBuffer.popSamples (inBuffer, 0, 0, internalBlocksize, 0);
            
            chunkMidiBuffer.clear();
            midiInputCollection.popEvents (chunkMidiBuffer,  internalBlocksize);
            
            if (isBypassed)
            {
                for (int chan = 0; chan < 2; ++chan)
                    outputBuffer.pushSamples (inBuffer, 0, 0, internalBlocksize, chan);
            }
            else
            {
                AudioBuffer<SampleType> outProxy = AudioBuffer<SampleType> (outBuffer.getArrayOfWritePointers(),
                                                                            outBuffer.getNumChannels(), 0, internalBlocksize);
                
                renderBlock (inBuffer, outProxy, chunkMidiBuffer);
                
                for (int chan = 0; chan < outBuffer.getNumChannels(); ++chan)
                    outputBuffer.pushSamples (outBuffer, chan, 0, internalBlocksize, chan);
            }
            
            midiOutputCollection.pushEvents (chunkMidiBuffer, internalBlocksize);
        }
        
        for (int chan = 0; chan < output.getNumChannels(); ++chan)
            outputBuffer.popSamples (output, chan, 0, numNewSamples, chan);
        
        midiOutputCollection.popEvents (midiMessages, numNewSamples);
        
        if (applyFadeIn)
            output.applyGainRamp (0, numNewSamples, 0.0f, 1.0f);
        
        if (applyFadeOut)
            output.applyGainRamp (0, numNewSamples, 1.0f, 0.0f);
    }
    
    
    template class FIFOWrappedEngineWithMidi<float>;
    template class FIFOWrappedEngineWithMidi<double>;
    
    
}  // namespace dsp

}  // namespace bav
