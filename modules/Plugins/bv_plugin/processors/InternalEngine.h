#pragma once

namespace bav::dsp
{
template < typename SampleType >
class ProcessorInternalEngine : public ParameterProcessorBase< SampleType >
{
public:
    ProcessorInternalEngine (juce::AudioProcessor& processorToUse,
                             PluginState&          stateToUse,
                             Engine< SampleType >& engineToUse);

    Engine< SampleType >* operator->() { return &engine; }

private:
    void renderChunk (juce::AudioBuffer< SampleType >& audio, juce::MidiBuffer& midi) final;

    juce::AudioProcessor& processor;
    PluginState&          state;
    Engine< SampleType >& engine;
};

}  // namespace bav::dsp
