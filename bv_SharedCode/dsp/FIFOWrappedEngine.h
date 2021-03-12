
namespace bav
{
    
namespace dsp
{
    
    using namespace juce;
    
    
template<typename SampleType>
class FIFOWrappedEngine
{
public:
    FIFOWrappedEngine(int consistentInternalBlocksize, double samplerate);
    
    FIFOWrappedEngine();
    
    virtual ~FIFOWrappedEngine();
    
    // these are the public functions your AudioProcessor should call directly from its processBlock, prepareToPlay, etc...
    
    void process (AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midiMessages, const bool isBypassed = false);
    
    void initialize (double samplerate, int newInternalBlocksize);
    
    void prepare (double samplerate);
    
    void reset();
    
    void releaseResources();
    
    void changeLatency (int newInternalBlocksize);
    
    int getLatency() const noexcept { return internalBlocksize; }
    
    bool hasBeenInitialized() const noexcept { return isInitialized; }
    bool hasBeenReleased() const noexcept { return resourcesReleased; }
    
    
private:
    
    // part of the FIFO process, not for public use
    void processWrapped (AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output,
                         MidiBuffer& midiMessages,
                         const bool applyFadeIn, const bool applyFadeOut,
                         const bool isBypassed = false);
    
    // these virtual functions should be overriden by your subclass to implement your engine's functionality:
    
    virtual void renderBlock (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midiMessages)=0;
    
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
    
}  // namespace dsp
    
}  // namespace bav
