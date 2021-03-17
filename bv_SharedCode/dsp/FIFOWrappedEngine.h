
namespace bav
{
    
namespace dsp
{
    
    using namespace juce;
    
    
template<typename SampleType>
class FIFOWrappedEngine
{
public:
    FIFOWrappedEngine(int consistentInternalBlocksize, double samplerate): internalBlocksize(0)
    {
        initialize (samplerate, consistentInternalBlocksize);
    }
    
    FIFOWrappedEngine(): internalBlocksize(0) { }
    
    virtual ~FIFOWrappedEngine() { }
    
    // these are the public functions your AudioProcessor should call directly from its processBlock, prepareToPlay, etc...
    
    void process (AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midiMessages, const bool isBypassed = false)
    {
        jassert (! resourcesReleased);
        jassert (isInitialized);
        
        const int totalNumSamples = input.getNumSamples();
        
        if (totalNumSamples == 0)
            return;
        
        jassert (totalNumSamples == output.getNumSamples());
        jassert (input.getNumChannels() == 2 && output.getNumChannels() == 2);
        
        bool processingBypassedThisFrame, applyFadeIn, applyFadeOut;
        
        if (isBypassed)
        {
            processingBypassedThisFrame = wasBypassedLastCallback;
            applyFadeIn = false;
            applyFadeOut = ! wasBypassedLastCallback;
        }
        else
        {
            processingBypassedThisFrame = false;
            applyFadeIn = wasBypassedLastCallback;
            applyFadeOut = false;
        }
        
        wasBypassedLastCallback = isBypassed;
        
        if (totalNumSamples <= internalBlocksize)
        {
            processWrapped (input, output, midiMessages, applyFadeIn, applyFadeOut, processingBypassedThisFrame);
            return;
        }
        
        int samplesLeft = totalNumSamples;
        int startSample = 0;
        
        do {
            const int chunkNumSamples = std::min (internalBlocksize, samplesLeft);
            
            AudioBuffer<SampleType> inputProxy  (input.getArrayOfWritePointers(),  2, startSample, chunkNumSamples);
            AudioBuffer<SampleType> outputProxy (output.getArrayOfWritePointers(), 2, startSample, chunkNumSamples);
            
            // put just the midi messages for this time segment into the midiChoppingBuffer, starting at timestamp 0
            midiChoppingBuffer.clear();
            bav::midi::copyRangeOfMidiBuffer (midiMessages, midiChoppingBuffer, startSample, 0, chunkNumSamples);
            
            processWrapped (inputProxy, outputProxy, midiChoppingBuffer, applyFadeIn, applyFadeOut, processingBypassedThisFrame);
            
            // copy the midi output back to midiMessages (I/O), at the original startSample
            bav::midi::copyRangeOfMidiBuffer (midiChoppingBuffer, midiMessages, 0, startSample, chunkNumSamples);
            
            startSample += chunkNumSamples;
            samplesLeft -= chunkNumSamples;
            
            applyFadeIn  = false;
            applyFadeOut = false;
            processingBypassedThisFrame = isBypassed;
        } while (samplesLeft > 0);
    }
    
    
    void process (AudioBuffer<SampleType>& inplaceInAndOut, MidiBuffer& midiMessages, const bool isBypassed = false)
    {
        process (inplaceInAndOut, inplaceInAndOut, midiMessages, isBypassed);
    }
    
    
    void initialize (double samplerate, int newInternalBlocksize)
    {
        jassert (samplerate > 0);
        jassert (newInternalBlocksize > 0);
        
        wasBypassedLastCallback = true;
        isInitialized = true;
        resourcesReleased = false;
        
        initialized (newInternalBlocksize, samplerate);
        
        changeLatency (newInternalBlocksize);
    }
    
    
    void prepare (double samplerate)
    {
        jassert (samplerate > 0);
        
        if (! isInitialized)
            initialize (samplerate, internalBlocksize);
        
        resourcesReleased = false;
        
        prepareToPlay (samplerate);
    }
    
    
    void reset()
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
    
    
    void releaseResources()
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
    
    
    void changeLatency (int newInternalBlocksize)
    {
        jassert (newInternalBlocksize > 0);
        
        internalBlocksize = newInternalBlocksize;
        
        inBuffer.setSize (2, internalBlocksize, true, true, true);
        outBuffer.setSize (2, internalBlocksize, true, true, true);
        
        const int doubleBlocksize = internalBlocksize * 2;
        
        inputBuffer.changeSize (2, doubleBlocksize);
        outputBuffer.changeSize(2, doubleBlocksize);
        
        const size_t doubleBlocksizeT = size_t(doubleBlocksize);
        
        midiInputCollection.setSize(doubleBlocksize);
        midiOutputCollection.setSize(doubleBlocksize);
        chunkMidiBuffer.ensureSize (doubleBlocksizeT);
        midiChoppingBuffer.ensureSize (doubleBlocksizeT);
        
        isInitialized = true;
        
        latencyChanged (newInternalBlocksize);
    }
    
    
    int getLatency() const noexcept { return internalBlocksize; }
    
    bool hasBeenInitialized() const noexcept { return isInitialized; }
    bool hasBeenReleased() const noexcept { return resourcesReleased; }
    
    
private:
    
    // part of the FIFO process, not for public use
    void processWrapped (AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output,
                         MidiBuffer& midiMessages,
                         const bool applyFadeIn, const bool applyFadeOut,
                         const bool isBypassed = false)
    {
        const int numNewSamples = input.getNumSamples();
        
        jassert (numNewSamples <= internalBlocksize && numNewSamples > 0);
        
        for (int chan = 0; chan < 2; ++chan)
            inputBuffer.pushSamples (input, chan, 0, numNewSamples, chan);
        
        midiInputCollection.pushEvents (midiMessages, numNewSamples);
        
        if (inputBuffer.numStoredSamples() >= internalBlocksize)  // we have enough samples, render the new chunk
        {
            inBuffer.clear();
            for (int chan = 0; chan < 2; ++chan)
                inputBuffer.popSamples (inBuffer, chan, 0, internalBlocksize, chan);
            
            chunkMidiBuffer.clear();
            midiInputCollection.popEvents (chunkMidiBuffer,  internalBlocksize);
            
            if (isBypassed)
            {
                for (int chan = 0; chan < 2; ++chan)
                    outputBuffer.pushSamples (inBuffer, chan, 0, internalBlocksize, chan);
                
                bypassedBlock (inBuffer, chunkMidiBuffer);
            }
            else
            {
                outBuffer.clear();
                
                renderBlock (inBuffer, outBuffer, chunkMidiBuffer);
                
                for (int chan = 0; chan < 2; ++chan)
                    outputBuffer.pushSamples (outBuffer, chan, 0, internalBlocksize, chan);
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
    
    
    // these virtual functions should be overriden by your subclass to implement your engine's functionality:
    
    virtual void renderBlock (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midiMessages)=0;
    
    virtual void bypassedBlock (const AudioBuffer<SampleType>& input, const MidiBuffer& midiMessages) { juce::ignoreUnused (input, midiMessages); }
    
    virtual void initialized (int newInternalBlocksize, double samplerate) { juce::ignoreUnused (newInternalBlocksize, samplerate); }
    
    virtual void prepareToPlay (double samplerate) { juce::ignoreUnused (samplerate); }
    
    virtual void resetTriggered() { }
    
    virtual void release() { }
    
    virtual void latencyChanged (int newInternalBlocksize) { juce::ignoreUnused(newInternalBlocksize); }
    
    int internalBlocksize; // the size of the processing blocks, in samples, that the algorithm will be processing
    
    juce::MidiBuffer midiChoppingBuffer;
    juce::MidiBuffer chunkMidiBuffer;
    bav::midi::MidiFIFO midiInputCollection;
    bav::midi::MidiFIFO midiOutputCollection;
    
    bav::dsp::AudioFIFO<SampleType> inputBuffer;
    bav::dsp::AudioFIFO<SampleType> outputBuffer;
    AudioBuffer<SampleType> inBuffer;
    AudioBuffer<SampleType> outBuffer;
    
    bool wasBypassedLastCallback = true;
    
    bool resourcesReleased = true;
    
    bool isInitialized = false;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FIFOWrappedEngine)
};
    

template class FIFOWrappedEngine<float>;
template class FIFOWrappedEngine<double>;

}  // namespace dsp
    
}  // namespace bav
