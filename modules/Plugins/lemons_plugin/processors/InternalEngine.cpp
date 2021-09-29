
namespace lemons::plugin
{
template < typename SampleType >
ProcessorInternalEngine< SampleType >::ParameterProcessor::ParameterProcessor (ProcessorInternalEngine& parent)
    : ParameterProcessorBase< SampleType > (parent.state.getParameters()),
      parentEngine (parent)
{
}

template < typename SampleType >
void ProcessorInternalEngine< SampleType >::ParameterProcessor::renderChunk (juce::AudioBuffer< SampleType >& audio, MidiBuffer& midi)
{
    parentEngine.modulationProcessor.processBlock (audio, midi);
}

/*------------------------------------------------------------------------------------------------------------*/

template < typename SampleType >
ProcessorInternalEngine< SampleType >::ModulationProcessor::ModulationProcessor (ProcessorInternalEngine& parent)
    : ModulationManagerProcessor< SampleType > (parent.state.modManager),
      parentEngine (parent)
{
}

template < typename SampleType >
void ProcessorInternalEngine< SampleType >::ModulationProcessor::renderChunk (juce::AudioBuffer< SampleType >& audio, MidiBuffer& midi)
{
    parentEngine.renderNextAudioSegment (audio, midi);
}

/*------------------------------------------------------------------------------------------------------------*/

template < typename SampleType >
ProcessorInternalEngine< SampleType >::ProcessorInternalEngine (juce::AudioProcessor&      processorToUse,
                                                                StateBase&                 stateToUse,
                                                                dsp::Engine< SampleType >& engineToUse)
    : processor (processorToUse), state (stateToUse), engine (engineToUse)
{
}

template < typename SampleType >
void ProcessorInternalEngine< SampleType >::prepareToPlay (double samplerate, int maxBlocksize)
{
    engine.prepare (samplerate, maxBlocksize);
    parameterProcessor.prepare (maxBlocksize);
}

template < typename SampleType >
void ProcessorInternalEngine< SampleType >::process (AudioBuffer< SampleType >& audio, MidiBuffer& midi)
{
    parameterProcessor.process (audio, midi);
}

inline int getIndexOfFirstValidChannelSet (const juce::AudioProcessor::BusesLayout& busLayout,
                                           bool                                     isInput)
{
    const auto numBuses = isInput ? busLayout.inputBuses.size() : busLayout.outputBuses.size();

    for (int i = 0; i < numBuses; ++i)
        if (! busLayout.getChannelSet (isInput, i).isDisabled())
            return i;

    return 0;
}

template < typename SampleType >
inline AudioBuffer< SampleType > findSubBuffer (juce::AudioProcessor&                    processor,
                                                const juce::AudioProcessor::BusesLayout& busLayout,
                                                juce::AudioBuffer< SampleType >&         origBuffer,
                                                bool                                     isInput)
{
    return processor.getBusBuffer (origBuffer, isInput,
                                   getIndexOfFirstValidChannelSet (busLayout, isInput));
}

template < typename SampleType >
void ProcessorInternalEngine< SampleType >::renderNextAudioSegment (AudioBuffer< SampleType >& audio, MidiBuffer& midi)
{
    const auto busLayout = processor.getBusesLayout();
    const auto inBus     = findSubBuffer (processor, busLayout, audio, true);
    auto       outBus    = findSubBuffer (processor, busLayout, audio, false);

    engine.process (inBus, outBus, midi, state.mainBypass->get());
}


template class ProcessorInternalEngine< float >;
template class ProcessorInternalEngine< double >;

}  // namespace bav::plugin
