
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

template < typename SampleType >
class FIFOWrappedEngine
{
    using AudioBuffer = juce::AudioBuffer< SampleType >;
    using MidiBuffer  = juce::MidiBuffer;


public:
    FIFOWrappedEngine (int consistentInternalBlocksize, double samplerate);

    FIFOWrappedEngine();

    virtual ~FIFOWrappedEngine() = default;


    // these are the public functions your AudioProcessor should call directly from its processBlock, prepareToPlay, etc...

    void process (AudioBuffer& inplaceInAndOut,
                  MidiBuffer&  midiMessages,
                  const bool   isBypassed = false);

    void process (AudioBuffer& input,
                  AudioBuffer& output,
                  MidiBuffer&  midiMessages,
                  const bool   isBypassed = false);
    
    // call these two functions to use the engine without MIDI
    // (the engine's internal renderBlock() will recieve an empty midiBuffer)
    
    void process (AudioBuffer& inplaceInAndOut,
                  const bool   isBypassed = false);
    
    void process (AudioBuffer& input,
                  AudioBuffer& output,
                  const bool   isBypassed = false);


    void initialize (double samplerate, int newInternalBlocksize);

    void prepare (double samplerate);

    void reset();

    void releaseResources();


    void changeLatency (int newInternalBlocksize);

    int getLatency() const noexcept { return internalBlocksize; }

    bool hasBeenInitialized() const noexcept { return isInitialized; }
    bool hasBeenReleased() const noexcept { return resourcesReleased; }

    double getSamplerate() const noexcept { return sampleRate; }


private:
    // part of the FIFO process, not for public use
    void processWrapped (AudioBuffer& input,
                         AudioBuffer& output,
                         MidiBuffer&  midiMessages,
                         const bool   applyFadeIn,
                         const bool   applyFadeOut,
                         const bool   isBypassed = false);


    // these virtual functions should be overriden by your subclass to implement your engine's functionality:

    virtual void renderBlock (const AudioBuffer& input,
                              AudioBuffer&       output,
                              MidiBuffer&        midiMessages) = 0;

    virtual void bypassedBlock (const AudioBuffer& input, MidiBuffer& midiMessages)
    {
        juce::ignoreUnused (input, midiMessages);
    }

    virtual void initialized (int newInternalBlocksize, double samplerate)
    {
        juce::ignoreUnused (newInternalBlocksize, samplerate);
    }

    virtual void prepareToPlay (double samplerate)
    {
        juce::ignoreUnused (samplerate);
    }

    virtual void resetTriggered() { }

    virtual void release() { }

    virtual void latencyChanged (int newInternalBlocksize)
    {
        juce::ignoreUnused (newInternalBlocksize);
    }

    int internalBlocksize;  // the size of the processing blocks, in samples, that the algorithm will be processing

    MidiBuffer midiChoppingBuffer;

    bav::dsp::AudioAndMidiFIFO< SampleType > inputFIFO;
    bav::dsp::AudioAndMidiFIFO< SampleType > outputFIFO;

    AudioBuffer inBuffer;
    AudioBuffer outBuffer;

    MidiBuffer chunkMidiBuffer;

    bool wasBypassedLastCallback = true;

    bool resourcesReleased = true;

    bool isInitialized = false;

    double sampleRate = 0;
    
    MidiBuffer dummyMidiBuffer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FIFOWrappedEngine)
};


}  // namespace bav::dsp
