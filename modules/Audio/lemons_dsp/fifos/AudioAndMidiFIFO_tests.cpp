#if ! LEMONS_UNIT_TESTS
#error
#endif

namespace lemons::tests
{

template <typename FloatType>
struct AudioAndMidiFifoTests final : public DspTest
{
public:
    explicit AudioAndMidiFifoTests()
    : DspTest (getDspTestName<FloatType> ("Audio and MIDI FIFO tests"))
    {
    }
    
private:
    void runTest() final
    {
        constexpr auto numChannels = 2;
        
        for (const auto numSamples : getTestingBlockSizes())
        {
            beginTest ("Blocksize: " + String (numSamples));
            
            resizeAllBuffers (numSamples, numChannels);
            
            fillAudioBufferWithRandomNoise (origAudio, getRandom());
            fillMidiBufferWithRandomEvents (origMidi, numSamples / 3, getRandom());
            
            fifo.push (origAudio, origMidi);
            
            logImportantMessage ("Num stored samples stored correctly");
            
            expectEquals (fifo.numStoredSamples(), numSamples);
            
            fifo.pop (audioOut, midiOut);
            
            expectEquals (fifo.numStoredSamples(), 0);
            
            logImportantMessage ("Store and retrieve audio and MIDI");
            
            expect (buffersAreEqual (audioOut, origAudio));
            expect (midiBuffersAreEqual (midiOut, origMidi));
            
            logImportantMessage ("Resizing clears the FIFO");
            
            fifo.push (origAudio, origMidi);
            
            fifo.setSize (numSamples / 2, numChannels + 1);
            
            expectEquals (fifo.numStoredSamples(), 0);
        }
    }
    
    void resizeAllBuffers (int newSize, int numChannels)
    {
        origAudio.setSize (numChannels, newSize, true, true, true);
        audioOut.setSize (numChannels, newSize, true, true, true);
        
        const auto messages = static_cast<size_t> (newSize);
        origMidi.ensureSize (messages);
        midiOut.ensureSize (messages);
        
        fifo.setSize (newSize, numChannels);
    }
    
    AudioBuffer<FloatType> origAudio, audioOut;
    
    MidiBuffer origMidi, midiOut;
    
    dsp::AudioAndMidiFIFO<FloatType> fifo;
};

template struct AudioAndMidiFifoTests<float>;
template struct AudioAndMidiFifoTests<double>;

LEMONS_CREATE_DSP_TEST (AudioAndMidiFifoTests)

}  // namespace lemons::tests
