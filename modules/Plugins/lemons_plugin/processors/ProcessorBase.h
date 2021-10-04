#pragma once

namespace lemons::plugin
{
class ProcessorBase : public dsp::BasicProcessorBase, private SystemInitializer
{
public:
    ProcessorBase (StateBase&                            stateToUse,
                   dsp::Engine< float >&                 floatEngineToUse,
                   dsp::Engine< double >&                doubleEngineToUse,
                   juce::AudioProcessor::BusesProperties busesLayout);

    void               saveEditorSize (int width, int height);
    juce::Point< int > getSavedEditorSize() const;

private:
    juce::AudioProcessorParameter* getBypassParameter() const final;

    void getStateInformation (juce::MemoryBlock& block) override;
    void setStateInformation (const void* data, int size) override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) final;

    void releaseResources() final;

    bool supportsDoublePrecisionProcessing() const final { return true; }

    void processBlock (AudioBuffer< float >& audio, MidiBuffer& midi) final;
    void processBlock (AudioBuffer< double >& audio, MidiBuffer& midi) final;
    void processBlockBypassed (AudioBuffer< float >& audio, MidiBuffer& midi) final;
    void processBlockBypassed (AudioBuffer< double >& audio, MidiBuffer& midi) final;

    /*-------------------------------------------------------*/

    template < typename SampleType >
    class InternalEngine : public ParameterProcessor< SampleType >
    {
    public:
        InternalEngine (juce::AudioProcessor& processorToUse, StateBase& stateToUse, dsp::Engine< SampleType >& engineToUse);

        void prepareToPlay (double samplerate, int maxBlocksize);

        dsp::Engine< SampleType >* operator->();

    private:
        void renderChunk (juce::AudioBuffer< SampleType >& audio, MidiBuffer& midi) final;

        juce::AudioProcessor&      processor;
        StateBase&                 state;
        dsp::Engine< SampleType >& engine;
    };

    /*-------------------------------------------------------*/

    template < typename SampleType1, typename SampleType2 >
    void prepareToPlayInternal (const double sampleRate, int samplesPerBlock,
                                InternalEngine< SampleType1 >& activeEngine,
                                InternalEngine< SampleType2 >& idleEngine);

    /*-------------------------------------------------------*/

    StateBase& state;

    InternalEngine< float >  floatEngine;
    InternalEngine< double > doubleEngine;
};

}  // namespace lemons::plugin
