
/** @ingroup BasicProcessor
 *  @{
 */

#pragma once


namespace lemons::dsp
{
/** A default implementation of a basic, generic juce AudioProcessor.
    The purpose of this class is to provide sensible default implementations for all of AudioProcessor's pure virtual functions, to make it easy to inherit from this class, override only what you need, and get working.
    @see plugin::ProcessorBase
 */
class BasicProcessorBase : public juce::AudioProcessor
{
public:
    /** Creates a basic processor.
        The default constructor initializes an audio processor with stereo in and out buses.
     */
    BasicProcessorBase (juce::AudioProcessor::BusesProperties busesLayout = BusesProperties()
                                                                                .withInput (TRANS ("Input"), juce::AudioChannelSet::stereo(), true)
                                                                                .withOutput (TRANS ("Output"), juce::AudioChannelSet::stereo(), true));

    /** Repaints the editor, if one exists. */
    void repaintEditor();

private:
    void prepareToPlay (double samplerate, int blocksize) override;
    void releaseResources() override;

    void processBlock (AudioBuffer< float >&, MidiBuffer&) override { }

    double getTailLengthSeconds() const override;

    void getStateInformation (juce::MemoryBlock&) override { }
    void setStateInformation (const void*, int) override { }

    int          getNumPrograms() override;
    int          getCurrentProgram() override;
    void         setCurrentProgram (int) override;
    const String getProgramName (int) override;
    void         changeProgramName (int, const String&) override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool supportsMPE() const override;
    bool isMidiEffect() const override;

    const String getName() const override;

    bool                        hasEditor() const override;
    juce::AudioProcessorEditor* createEditor() override;

    bool isBusesLayoutSupported (const BusesLayout& layout) const override;
};


}  // namespace lemons::dsp

/** @}*/
