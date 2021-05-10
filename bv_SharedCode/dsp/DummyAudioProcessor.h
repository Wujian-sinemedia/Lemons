
namespace bav::dsp
{


class DummyAudioProcessor    : public  juce::AudioProcessor
{
public:
    DummyAudioProcessor();
    ~DummyAudioProcessor() override;
    
    /*=========================================================================================*/
    
    void prepareToPlay (double, int) override final { }
    void releaseResources() override final { }
    
    void processBlock (juce::AudioBuffer<float>&,  juce::MidiBuffer&) override final { }
    
    double getTailLengthSeconds() const override final;
    
    void getStateInformation (juce::MemoryBlock&) override final { }
    void setStateInformation (const void*, int) override final { }
    
    int getNumPrograms() override final { return 1; }
    int getCurrentProgram() override final { return 0; }
    void setCurrentProgram (int) override final { }
    const juce::String getProgramName (int) override final { return {}; }
    void changeProgramName (int, const juce::String&) override final { }
    
    bool acceptsMidi()  const override final { return true;  }
    bool producesMidi() const override final { return true;  }
    bool supportsMPE()  const override final { return false; }
    bool isMidiEffect() const override final { return false; }
    
    const juce::String getName() const override final { return "DummyAudioProcessor"; }
   
    bool hasEditor() const override final { return false; }
    juce::AudioProcessorEditor* createEditor() override final { return nullptr; }
    
    /*=========================================================================================*/
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ImogenAudioProcessor)
};


}  // namespace
