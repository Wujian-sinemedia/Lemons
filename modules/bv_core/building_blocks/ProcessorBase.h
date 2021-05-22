
#pragma once


namespace bav::dsp
{

class ProcessorBase :   private SystemInitializer,
                        public juce::AudioProcessor
{
public:
    ProcessorBase()
    : AudioProcessor (createBusProperties())
    { }
    
    virtual ~ProcessorBase() override { }

    /*=========================================================================================*/

    void prepareToPlay (double, int) override { }
    void releaseResources() override { }

    void processBlock (juce::AudioBuffer< float >&, juce::MidiBuffer&) override { }

    double getTailLengthSeconds() const override { return 0.0; }

    void getStateInformation (juce::MemoryBlock&) override { }
    void setStateInformation (const void*, int) override { }

    int                getNumPrograms() override { return 1; }
    int                getCurrentProgram() override { return 0; }
    void               setCurrentProgram (int) override { }
    const juce::String getProgramName (int) override { return {}; }
    void               changeProgramName (int, const juce::String&) override { }

    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return true; }
    bool supportsMPE() const override { return false; }
    bool isMidiEffect() const override { return false; }

    const juce::String getName() const override { return "ProcessorBase"; }

    bool                        hasEditor() const override { return true; }
    juce::AudioProcessorEditor* createEditor() override { return new EditorBase(*this); }
    
    bool isBusesLayoutSupported(const BusesLayout& layout) const override
    {
        using Set = juce::AudioChannelSet;
        
        auto isValid = [](const Set& set) { return set != Set::disabled(); };
        
        return isValid (layout.getMainInputChannelSet()) && isValid (layout.getMainOutputChannelSet());
    }
    
    virtual void saveEditorSize (int width, int height)
    {
        savedEditorSize.x = width;
        savedEditorSize.y = height;
    }
    
    virtual juce::Point<int> getSavedEditorSize() const { return savedEditorSize; }

    /*=========================================================================================*/

protected:
    virtual juce::AudioProcessor::BusesProperties createBusProperties() const
    {
        const auto stereo = juce::AudioChannelSet::stereo();
        
        return BusesProperties().withInput (TRANS ("Input"), stereo, true)
                                .withOutput (TRANS ("Output"), stereo, true);
    }
    
    juce::Point<int> savedEditorSize;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DummyAudioProcessor)
};


}  // namespace bav::dsp
