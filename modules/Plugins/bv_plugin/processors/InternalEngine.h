#pragma once

namespace bav::plugin
{
template < typename SampleType >
class ProcessorInternalEngine
{
public:
    ProcessorInternalEngine (juce::AudioProcessor&      processorToUse,
                             StateBase&                 stateToUse,
                             dsp::Engine< SampleType >& engineToUse);

    dsp::Engine< SampleType >* operator->() { return &engine; }

    void prepareToPlay (double samplerate, int maxBlocksize);

    void process (AudioBuffer< SampleType >& audio, MidiBuffer& midi);

private:
    class ParameterProcessor : public ParameterProcessorBase< SampleType >
    {
    public:
        ParameterProcessor (ProcessorInternalEngine& parent);

    private:
        void renderChunk (juce::AudioBuffer< SampleType >& audio, MidiBuffer& midi) final;

        ProcessorInternalEngine& parentEngine;
    };

    class ModulationProcessor : public ModulationManagerProcessor< SampleType >
    {
    public:
        ModulationProcessor (ProcessorInternalEngine& parent);

    private:
        ProcessorInternalEngine& parentEngine;
    };

    /*-----------------------------------------------*/

    void renderNextAudioSegment (AudioBuffer< SampleType >& audio, MidiBuffer& midi);

    /*-----------------------------------------------*/

    juce::AudioProcessor&      processor;
    StateBase&                 state;
    dsp::Engine< SampleType >& engine;

    ParameterProcessor  parameterProcessor {*this};
    ModulationProcessor modulationProcessor {*this};
};

}  // namespace bav::plugin
