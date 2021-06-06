
#pragma once


namespace bav::dsp
{
class ProcessorBase : private SystemInitializer,
                      public juce::AudioProcessor
{
public:
    ProcessorBase();
    virtual ~ProcessorBase() override;

    void prepareToPlay (double, int) override;
    void releaseResources() override;

    void processBlock (juce::AudioBuffer< float >& audio, juce::MidiBuffer& midi) final;
    void processBlock (juce::AudioBuffer< double >& audio, juce::MidiBuffer& midi) final;
    void processBlockBypassed (juce::AudioBuffer< float >& audio, juce::MidiBuffer& midi) final;
    void processBlockBypassed (juce::AudioBuffer< double >& audio, juce::MidiBuffer& midi) final;

    double getTailLengthSeconds() const override;

    void getStateInformation (juce::MemoryBlock&) final;
    void setStateInformation (const void*, int) final;

    int                getNumPrograms() override;
    int                getCurrentProgram() override;
    void               setCurrentProgram (int) override;
    const String getProgramName (int) override;
    void               changeProgramName (int, const String&) override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool supportsMPE() const override;
    bool isMidiEffect() const override;

    const String getName() const override;

    bool                        hasEditor() const override;
    juce::AudioProcessorEditor* createEditor() override;
    
    juce::AudioProcessorParameter* getBypassParameter() const final;

    bool isBusesLayoutSupported (const BusesLayout& layout) const override;

    void               saveEditorSize (int width, int height);
    juce::Point< int > getSavedEditorSize() const;
    void               getSavedEditorSize (int& width, int& height) const;
    
    void repaintEditor();

    /*=========================================================================================*/

private:
    virtual juce::AudioProcessor::BusesProperties createBusProperties() const;
    
    virtual void processBlockInternal (juce::AudioBuffer< float >&, juce::MidiBuffer&) { }
    virtual void processBlockInternal (juce::AudioBuffer< double >&, juce::MidiBuffer&) { }
    
    virtual SerializableData* getStateData() { return nullptr; }
    
    virtual bav::BoolParameter* getMainBypass() const { return nullptr; }
    
    juce::Point< int > savedEditorSize {450, 300};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProcessorBase)
};


}  // namespace bav::dsp
