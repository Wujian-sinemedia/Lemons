
namespace bav::dsp
{
    
    
    /*
     Base class for an audio engine that requires a consistent blocksize, or certain amount of latency, for its internal processing.
     The blocksize that is processed internally is equal to the latency in samples of the engine. They are one and the same.
     
     *
     
     To create your own engine using this base class, create a subclass that inherits from this class, and override the following private functions:
     
     void renderBlock (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midiMessages)
     - REQUIRED for any class that inherits from this base class!!!
     - this is the function that will be called with a regulated blocksize. The number of samples in the input and output buffers will always be the engine's internal latency, as set by the last call to changeLatency()
     - as of now, the input and output buffers will always have two (2) channels if a buffer with two or more channels is sent into process(). If a mono buffer is sent into process(), renderBlock() will recieve a mono buffer.
     - your engine's midi output should be returned by copying it back to midiMessages. Simply do nothing with midiMessages for an engine that does not output midi.
     
     void bypassedBlock (const AudioBuffer<SampleType>& input, MidiBuffer& midiMessages)
     - called when your engine is in a bypassed state. You don't need to (or get to!) output any audio in this function, but you may alter the contents of the midiMessages buffer.
     - This function is useful, for example, if your engine contains some smoothed values, so in your implementation of your engine's bypassedBlock, you can call smoothedValue.skip(input.getNumSamples()).
     
     void initialized (int newInternalBlocksize, double samplerate)
     - Called when the top-level call to the engine's public initialize() function is made. Should be used to allocate any memory your processing may need, and resize any buffers. The newInternalBlocksize argument is guarunteed to be the same as the number of samples that will be sent to the next call of renderBlock().
     - Guarunteed to be called before any of the other functions on this list [with the only exceptions possibly being resetTriggered() or relaese()]. If a call to prepare() or changeLatency() is recieved before initialized() has been called, the engine base class will call your implementation of initialized() before calling the originally requested function.
     
     void prepareToPlay (double samplerate)
     - Called when the top-level call to the engine's public prepare() function is made.
     - This function does not have a blocksize argument, because any time the internal blocksize of the engine changes, latencyChanged() will be called. Any initialization or reallocations dependant on blocksize should be done in latencyChanged().
     
     void resetTriggered()
     - Called when the top-level call to the engine's public reset() function is made.
     - You should not free or deallocate memory in this function; recommended use is clearing buffers or resetting ramped values, etc.
     
     void release()
     - Called when the top-level call to the engine's public releaseResources() function is made.
     - When this function is called, think of it like the caller asking for your engine to have 0 memory and CPU footprint once this function has returned.
     - After this function has been called, initialized() is guarunteed to be called again before any of the other functions in this list.
     
     void latencyChanged (int newInternalBlocksize)
     - Called when the top-level call to the engine's public changeLatency() function is made. The newInternalBlocksize argument is guarunteed to be equal to the number of samples sent to your engine in the next call of renderBlock().
     
     *
     
     To use an audio engine derived from this base class, call the following public functions directly from your AudioProcessor or AudioAppComponent:
     
     void process (AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midiMessages, const bool isBypassed = false)
     - Can be called with any blocksize, even 0 samples, and the engine's internal consistent blocksize processing will be preserved.
     - use the isBypassed argument to have the engine automatically provide anti-artefact fade ins and outs when the bypass state is changed.
     - The engine's midi output will be returned in-place in the midiMessages buffer sent to this function.
     
     void process (AudioBuffer<SampleType>& inplaceInAndOut, MidiBuffer& midiMessages, const bool isBypassed = false)
     - The same as the proess() above, but returns the audio output in-place instead of to a different output buffer.
     
     void initialize (double samplerate, int newInternalBlocksize)
     - You should call this function before attempting to call any other function of your engine object.
     
     void prepare (double samplerate)
     
     void reset()
     - This function does not free or deallocate memory; it clears buffers, resets ramped values, etc.
     
     void releaseResources()
     - After calling this function, the engine object will have zero memory and CPU footprint.
     
     void changeLatency (int newInternalBlocksize)
     - Call this function to change the size of its internally processed blocks.
     */
    
    template<typename SampleType>
    class FIFOWrappedEngine
    {
        
        using AudioBuffer = juce::AudioBuffer<SampleType>;
        using MidiBuffer  = juce::MidiBuffer;
        
        
    public:
        FIFOWrappedEngine(int consistentInternalBlocksize, double samplerate): internalBlocksize(0)
        {
            initialize (samplerate, consistentInternalBlocksize);
        }
        
        FIFOWrappedEngine(): internalBlocksize(0) { }
        
        virtual ~FIFOWrappedEngine() { }
        
        // these are the public functions your AudioProcessor should call directly from its processBlock, prepareToPlay, etc...
        
        void process (AudioBuffer& input, AudioBuffer& output, MidiBuffer& midiMessages, const bool isBypassed = false)
        {
            jassert (! resourcesReleased);
            jassert (isInitialized);
            jassert (sampleRate > 0);
            
            const auto totalNumSamples = input.getNumSamples();
            
            if (totalNumSamples == 0)
                return;
            
            jassert (totalNumSamples == output.getNumSamples());
            
            const auto numInChannels  = std::min (2, input.getNumChannels());
            const auto numOutChannels = std::min (2, output.getNumChannels());
            
            if (numInChannels == 0 || numOutChannels == 0)
                return;
                
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
                const auto chunkNumSamples = std::min (internalBlocksize, samplesLeft);
                
                AudioBuffer inputProxy  (input.getArrayOfWritePointers(),  numInChannels,  startSample, chunkNumSamples);
                AudioBuffer outputProxy (output.getArrayOfWritePointers(), numOutChannels, startSample, chunkNumSamples);
                
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
        
        
        void process (AudioBuffer& inplaceInAndOut, MidiBuffer& midiMessages, const bool isBypassed = false)
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
            
            sampleRate = samplerate;
            
            if (! isInitialized)
            {
                jassert (internalBlocksize > 0); // if you hit this assertion, you probably made a call to prepare() before calling initialize().
                initialize (samplerate, internalBlocksize);
            }
            
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
            inBuffer.setSize (0, 0, false, false, false);
            outBuffer.setSize (0, 0, false, false, false);
            
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
            
            inBuffer.setSize  (2, internalBlocksize, true, true, true);
            outBuffer.setSize (2, internalBlocksize, true, true, true);
            
            const auto doubleBlocksize = internalBlocksize * 2;
            
            inputBuffer.changeSize (2, doubleBlocksize);
            outputBuffer.changeSize(2, doubleBlocksize);
            
            const auto doubleBlocksizeT = size_t(doubleBlocksize);
            
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
        
        double getSamplerate() const noexcept { return sampleRate; }
        
        
    private:
        
        // part of the FIFO process, not for public use
        void processWrapped (AudioBuffer& input, AudioBuffer& output,
                             MidiBuffer& midiMessages,
                             const bool applyFadeIn, const bool applyFadeOut,
                             const bool isBypassed = false)
        {
            const auto numNewSamples = input.getNumSamples();
            jassert (numNewSamples <= internalBlocksize && numNewSamples > 0);
            
            const auto numInChannels  = input.getNumChannels();
            const auto numOutChannels = output.getNumChannels();
            
            jassert (numInChannels <= 2 && numOutChannels <= 2);
            
            for (int chan = 0; chan < numInChannels; ++chan)
                inputBuffer.pushSamples (input, chan, 0, numNewSamples, chan);
            
            midiInputCollection.pushEvents (midiMessages, numNewSamples);
            
            if (inputBuffer.numStoredSamples() >= internalBlocksize)  // we have enough samples, render the new chunk
            {
                inBuffer.clear();
                
                for (int chan = 0; chan < numInChannels; ++chan)
                    inputBuffer.popSamples (inBuffer, chan, 0, internalBlocksize, chan);
                
                chunkMidiBuffer.clear();
                midiInputCollection.popEvents (chunkMidiBuffer,  internalBlocksize);
                
                if (isBypassed)
                {
                    for (int chan = 0; chan < numOutChannels; ++chan)
                        outputBuffer.pushSamples (inBuffer, chan, 0, internalBlocksize, chan);
                    
                    bypassedBlock (inBuffer, chunkMidiBuffer);
                }
                else
                {
                    outBuffer.clear();
                    
                    renderBlock (inBuffer, outBuffer, chunkMidiBuffer);
                    
                    for (int chan = 0; chan < numOutChannels; ++chan)
                        outputBuffer.pushSamples (outBuffer, chan, 0, internalBlocksize, chan);
                }
                
                midiOutputCollection.pushEvents (chunkMidiBuffer, internalBlocksize);
            }
            
            for (int chan = 0; chan < numOutChannels; ++chan)
                outputBuffer.popSamples (output, chan, 0, numNewSamples, chan);
            
            midiOutputCollection.popEvents (midiMessages, numNewSamples);
            
            if (applyFadeIn)
                output.applyGainRamp (0, numNewSamples, 0.0f, 1.0f);
            
            if (applyFadeOut)
                output.applyGainRamp (0, numNewSamples, 1.0f, 0.0f);
        }
        
        
        // these virtual functions should be overriden by your subclass to implement your engine's functionality:
        
        virtual void renderBlock (const AudioBuffer& input, AudioBuffer& output, MidiBuffer& midiMessages)=0;
        
        virtual void bypassedBlock (const AudioBuffer& input, MidiBuffer& midiMessages) { juce::ignoreUnused (input, midiMessages); }
        
        virtual void initialized (int newInternalBlocksize, double samplerate) { juce::ignoreUnused (newInternalBlocksize, samplerate); }
        
        virtual void prepareToPlay (double samplerate) { juce::ignoreUnused (samplerate); }
        
        virtual void resetTriggered() { }
        
        virtual void release() { }
        
        virtual void latencyChanged (int newInternalBlocksize) { juce::ignoreUnused(newInternalBlocksize); }
        
        int internalBlocksize; // the size of the processing blocks, in samples, that the algorithm will be processing
        
        MidiBuffer midiChoppingBuffer;
        MidiBuffer chunkMidiBuffer;
        bav::midi::MidiFIFO midiInputCollection;
        bav::midi::MidiFIFO midiOutputCollection;
        
        bav::dsp::AudioFIFO<SampleType> inputBuffer;
        bav::dsp::AudioFIFO<SampleType> outputBuffer;
        AudioBuffer inBuffer;
        AudioBuffer outBuffer;
        
        bool wasBypassedLastCallback = true;
        
        bool resourcesReleased = true;
        
        bool isInitialized = false;
        
        double sampleRate = 0;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FIFOWrappedEngine)
    };
    
    
    template class FIFOWrappedEngine<float>;
    template class FIFOWrappedEngine<double>;
    
    
}  // namespace

