
namespace bav::dsp
{
ProcessorBase::ProcessorBase (ParameterList&                        parameterList,
                              Engine< float >&                      floatEngineToUse,
                              Engine< double >&                     doubleEngineToUse,
                              juce::AudioProcessor::BusesProperties busesLayout)
    : BasicProcessorBase (busesLayout),
      parameters (parameterList),
      floatEngine (floatEngineToUse),
      doubleEngine (doubleEngineToUse)
{
    parameters.addDataChild (lastSavedEditorSize);
}

template < typename SampleType >
ProcessorBase::ParameterProcessor< SampleType >::ParameterProcessor (ProcessorBase& p, ParameterList& l)
    : ParameterProcessorBase< SampleType > (l),
      processor (p)
{
}

ProcessorBase::LastSavedEditorSize::LastSavedEditorSize (ProcessorBase& b)
    : SerializableData ("LastSavedEditorSize"),
      base (b)
{
}

void ProcessorBase::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    if (isUsingDoublePrecision())
        prepareToPlayInternal (sampleRate, samplesPerBlock, doubleEngine, floatEngine);
    else
        prepareToPlayInternal (sampleRate, samplesPerBlock, floatEngine, doubleEngine);
}

template < typename SampleType1, typename SampleType2 >
void ProcessorBase::prepareToPlayInternal (const double           sampleRate,
                                           int                    samplesPerBlock,
                                           Engine< SampleType1 >& activeEngine,
                                           Engine< SampleType2 >& idleEngine)
{
    if (idleEngine.isInitialized())
        idleEngine.releaseResources();

    activeEngine.prepare (sampleRate, samplesPerBlock);

    setLatencySamples (activeEngine.reportLatency());
}

void ProcessorBase::releaseResources()
{
    doubleEngine.releaseResources();
    floatEngine.releaseResources();
}

void ProcessorBase::getStateInformation (juce::MemoryBlock& block)
{
    serializing::toBinary (parameters, block);
}

void ProcessorBase::LastSavedEditorSize::toValueTree (ValueTree& tree)
{
    const auto size = base.getSavedEditorSize();
    tree.setProperty ("editorSizeX", size.x, nullptr);
    tree.setProperty ("editorSizeY", size.y, nullptr);
}

void ProcessorBase::setStateInformation (const void* data, int size)
{
    serializing::fromBinary (data, size, parameters);
}

void ProcessorBase::LastSavedEditorSize::fromValueTree (const ValueTree& tree)
{
    base.saveEditorSize (tree.getProperty ("editorSizeX"),
                         tree.getProperty ("editorSizeY"));
}

void ProcessorBase::processBlock (juce::AudioBuffer< float >& audio, juce::MidiBuffer& midi)
{
    processBlockInternal (audio, midi, floatParameterProcessor);
}

void ProcessorBase::processBlock (juce::AudioBuffer< double >& audio, juce::MidiBuffer& midi)
{
    processBlockInternal (audio, midi, doubleParameterProcessor);
}

void ProcessorBase::processBlockBypassed (juce::AudioBuffer< float >& audio, juce::MidiBuffer& midi)
{
    getMainBypass().set (true);
    processBlockInternal (audio, midi, floatParameterProcessor);
}

void ProcessorBase::processBlockBypassed (juce::AudioBuffer< double >& audio, juce::MidiBuffer& midi)
{
    getMainBypass().set (true);
    processBlockInternal (audio, midi, doubleParameterProcessor);
}

template < typename SampleType >
void ProcessorBase::processBlockInternal (juce::AudioBuffer< SampleType >&  audio,
                                          juce::MidiBuffer&                 midi,
                                          ParameterProcessor< SampleType >& parameterProcessor)
{
    juce::ScopedNoDenormals nodenorms;
    parameterProcessor.process (audio, midi);
}

template <>
void ProcessorBase::ParameterProcessor< float >::renderChunk (juce::AudioBuffer< float >& audio, juce::MidiBuffer& midi)
{
    processor.renderChunk (processor.floatEngine, audio, midi);
}

template <>
void ProcessorBase::ParameterProcessor< double >::renderChunk (juce::AudioBuffer< double >& audio, juce::MidiBuffer& midi)
{
    processor.renderChunk (processor.doubleEngine, audio, midi);
}

template < typename SampleType >
void ProcessorBase::renderChunk (Engine< SampleType >& engine, juce::AudioBuffer< SampleType >& audio, juce::MidiBuffer& midi)
{
    const auto busLayout = getBusesLayout();
    const auto inBus     = findSubBuffer (busLayout, audio, true);
    auto       outBus    = findSubBuffer (busLayout, audio, false);

    engine.process (inBus, outBus, midi, getMainBypass().get());
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
juce::AudioBuffer< SampleType > ProcessorBase::findSubBuffer (const AudioProcessor::BusesLayout& busLayout,
                                                              juce::AudioBuffer< SampleType >&   origBuffer,
                                                              bool                               isInput)
{
    return getBusBuffer (origBuffer, isInput,
                         getIndexOfFirstValidChannelSet (busLayout, isInput));
}

template struct ProcessorBase::ParameterProcessor< float >;
template struct ProcessorBase::ParameterProcessor< double >;

}  // namespace bav::dsp
