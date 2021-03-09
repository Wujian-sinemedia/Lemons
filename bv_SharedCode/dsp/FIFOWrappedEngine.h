
namespace bav
{
    
namespace dsp
{
    
    using namespace juce;
    
/*
    Base class for an audio processor wrapped in a FIFO (audio only; for MIDI features, use FIFOWrappedEngineWithMidi).
*/

template<typename SampleType>
class FIFOWrappedEngine
{
public:
    FIFOWrappedEngine(int internalBlocksize);
    virtual ~FIFOWrappedEngine();
    
    // these are the public functions your AudioProcessor should call directly from its processBlock, prepareToPlay, etc...
    
    void process (AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, const bool isBypassed = false);
    
    void prepare (double samplerate, int blocksize);
    
    
    void releaseResources();
    
    void changeLatency (int newInternalBlocksize);
    
    int getLatency() const noexcept { return internalBlocksize; }
    
    
private:
    
    // part of the FIFO process, not for public use
    void processWrapped (AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output,
                         const bool applyFadeIn, const bool applyFadeOut,
                         const bool isBypassed = false);
    
    // these virtual functions should be overriden by your subclass to implement your engine's functionality:
    
    virtual void renderBlock (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output)=0;
    
    virtual void renderBypassedBlock (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midiMessages);
    
    virtual void prepareToPlay (double samplerate, int blocksize) { juce::ignoreUnused (samplerate, blocksize); }
    
    virtual void release() { }
    
    virtual void latencyChanged (int newInternalBlocksize) { juce::ignoreUnused(newInternalBlocksize); }
    
    int internalBlocksize; // the size of the processing blocks, in samples, that the algorithm will be processing
    
    bav::dsp::AudioFIFO<SampleType> inputBuffer;
    bav::dsp::AudioFIFO<SampleType> outputBuffer;
    AudioBuffer<SampleType> inBuffer;
    AudioBuffer<SampleType> outBuffer;
    
    bool wasBypassedLastCallback;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FIFOWrappedEngine)
};
    
    
    
/*
    Base class for an audio processor wrapped in FIFOs, with audio and midi.
*/

template<typename SampleType>
class FIFOWrappedEngineWithMidi
{
public:
    FIFOWrappedEngineWithMidi(int internalBlocksize);
    virtual ~FIFOWrappedEngineWithMidi();
    
    // these are the public functions your AudioProcessor should call directly from its processBlock, prepareToPlay, etc...
    
    void process (AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midiMessages, const bool isBypassed = false);
    
    void prepare (double samplerate, int blocksize);
    
    
    void releaseResources();
    
    void changeLatency (int newInternalBlocksize);
    
    int getLatency() const noexcept { return internalBlocksize; }
    
    
private:
    
    // part of the FIFO process, not for public use
    void processWrapped (AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output,
                         MidiBuffer& midiMessages,
                         const bool applyFadeIn, const bool applyFadeOut,
                         const bool isBypassed = false);
    
    // these virtual functions should be overriden by your subclass to implement your engine's functionality:
    
    virtual void renderBlock (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midiMessages)=0;
    
    virtual void renderBypassedBlock (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midiMessages);
    
    virtual void prepareToPlay (double samplerate, int blocksize) { juce::ignoreUnused (samplerate, blocksize); }
    
    virtual void release() { }
    
    virtual void latencyChanged (int newInternalBlocksize) { juce::ignoreUnused(newInternalBlocksize); }
    
    int internalBlocksize; // the size of the processing blocks, in samples, that the algorithm will be processing
    
    juce::MidiBuffer midiChoppingBuffer;
    bav::midi::MidiFIFO midiInputCollection;
    bav::midi::MidiFIFO midiOutputCollection;
    bav::midi::MidiFIFO chunkMidiBuffer;
    
    bav::dsp::AudioFIFO<SampleType> inputBuffer;
    bav::dsp::AudioFIFO<SampleType> outputBuffer;
    AudioBuffer<SampleType> inBuffer;
    AudioBuffer<SampleType> outBuffer;
    
    bool wasBypassedLastCallback;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FIFOWrappedEngineWithMidi)
};
    
}  // namespace dsp
    
}  // namespace bav
