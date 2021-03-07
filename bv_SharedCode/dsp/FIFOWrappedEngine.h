
namespace bav
{
    
namespace dsp
{
    
    using namespace juce;
    

template<typename SampleType>
class FIFOWrappedEngine
{
public:
    FIFOWrappedEngine(int internalBlocksize);
    virtual ~FIFOWrappedEngine();
    
    
    void process (AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midiMessages,
                  const bool applyFadeIn = false, const bool applyFadeOut = false,
                  const bool isBypassed = false);
    
    
    void prepare (double samplerate, int blocksize);
    
    
    void releaseResources();
    
    void changeLatency (int newInternalBlocksize);
    
    int getLatency() const noexcept { return internalBlocksize; }
    
    
private:
    
    void processWrapped (AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output,
                         MidiBuffer& midiMessages,
                         const bool applyFadeIn, const bool applyFadeOut,
                         const bool isBypassed = false);
    
    virtual void renderBlock (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midiMessages)=0;
    
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
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FIFOWrappedEngine)
};
    
}  // namespace dsp
    
}  // namespace bav
