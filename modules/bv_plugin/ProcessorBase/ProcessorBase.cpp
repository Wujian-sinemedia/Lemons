
namespace bav::dsp
{
void ProcessorBase::getStateInformation (juce::MemoryBlock& block)
{
    serializing::toBinary (getStateData(), block);
}

void ProcessorBase::setStateInformation (const void* data, int size)
{
    serializing::fromBinary (data, size, getStateData());
    repaintEditor();
}

void ProcessorBase::processBlock (juce::AudioBuffer< float >& audio, juce::MidiBuffer& midi)
{
    processBlockInternal (audio, midi);
}

void ProcessorBase::processBlock (juce::AudioBuffer< double >& audio, juce::MidiBuffer& midi)
{
    processBlockInternal (audio, midi);
}

void ProcessorBase::processBlockBypassed (juce::AudioBuffer< float >& audio, juce::MidiBuffer& midi)
{
    auto& bypass = getMainBypass();
    if (! bypass.get())
        bypass.set (true);

    processBlockInternal (audio, midi);
}

void ProcessorBase::processBlockBypassed (juce::AudioBuffer< double >& audio, juce::MidiBuffer& midi)
{
    auto& bypass = getMainBypass();
    if (! bypass.get())
        bypass.set (true);

    processBlockInternal (audio, midi);
}

template < typename SampleType >
void ProcessorBase::processBlockInternal (juce::AudioBuffer< SampleType >& audio, juce::MidiBuffer& midi)
{
    juce::ScopedNoDenormals nodenorms;
    parameterProcessor.process (audio, midi);
}

ProcessorBase::ParameterProcessor::ParameterProcessor (ProcessorBase& p, ParameterList& l)
    : ParameterProcessorBase (l),
      processor (p)
{
}

void ProcessorBase::ParameterProcessor::renderChunk (juce::AudioBuffer< float >& audio, juce::MidiBuffer& midi)
{
    processor.renderChunk (audio, midi);
}

void ProcessorBase::ParameterProcessor::renderChunk (juce::AudioBuffer< double >& audio, juce::MidiBuffer& midi)
{
    processor.renderChunk (audio, midi);
}

}  // namespace bav::dsp
