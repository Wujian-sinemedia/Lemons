
namespace bav::dsp
{


class DummyAudioProcessor    : public  juce::AudioProcessor
{
public:
    DummyAudioProcessor() { }
    ~DummyAudioProcessor() override { }
    
    /*=========================================================================================*/
    
    void prepareToPlay (double, int) override { }
    void releaseResources() override { }
    
    void processBlock (juce::AudioBuffer<float>&,  juce::MidiBuffer&) override { }
    
    double getTailLengthSeconds() const override { return 0.0; }
    
    void getStateInformation (juce::MemoryBlock&) override { }
    void setStateInformation (const void*, int) override { }
    
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override { }
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override { }
    
    bool acceptsMidi()  const override { return true;  }
    bool producesMidi() const override { return true;  }
    bool supportsMPE()  const override { return false; }
    bool isMidiEffect() const override { return false; }
    
    const juce::String getName() const override { return TRANS ("Dummy AudioProcessor"); }
   
    bool hasEditor() const override { return false; }
    juce::AudioProcessorEditor* createEditor() override { return nullptr; }
    
    /*=========================================================================================*/
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DummyAudioProcessor)
};


}  // namespace
