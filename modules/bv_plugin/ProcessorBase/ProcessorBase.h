
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

    void processBlock (juce::AudioBuffer< float >&, juce::MidiBuffer&) override;

    double getTailLengthSeconds() const override;

    void getStateInformation (juce::MemoryBlock&) override;
    void setStateInformation (const void*, int) override;

    int                getNumPrograms() override;
    int                getCurrentProgram() override;
    void               setCurrentProgram (int) override;
    const juce::String getProgramName (int) override;
    void               changeProgramName (int, const juce::String&) override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool supportsMPE() const override;
    bool isMidiEffect() const override;

    const juce::String getName() const override;

    bool                        hasEditor() const override;
    juce::AudioProcessorEditor* createEditor() override;

    bool isBusesLayoutSupported (const BusesLayout& layout) const override;

    void               saveEditorSize (int width, int height);
    juce::Point< int > getSavedEditorSize() const;
    void               getSavedEditorSize (int& width, int& height) const;

    /*=========================================================================================*/

protected:
    virtual juce::AudioProcessor::BusesProperties createBusProperties() const;

    juce::Point< int > savedEditorSize {450, 300};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProcessorBase)
};


}  // namespace bav::dsp
