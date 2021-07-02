
namespace bav::dsp
{
ProcessorBase::ProcessorBase (PluginState&                          stateToUse,
                              Engine< float >&                      floatEngineToUse,
                              Engine< double >&                     doubleEngineToUse,
                              juce::AudioProcessor::BusesProperties busesLayout)
    : BasicProcessorBase (busesLayout),
      state (stateToUse),
      floatEngine (floatEngineToUse),
      doubleEngine (doubleEngineToUse)
{
}

ProcessorBase::LastSavedEditorSize::LastSavedEditorSize (ProcessorBase& b)
    : SerializableData ("LastSavedEditorSize"),
      base (b)
{
}

juce::AudioProcessorParameter* ProcessorBase::getBypassParameter() const
{
    return state.mainBypass.get();
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

    jassert (sampleRate > 0 && samplesPerBlock > 0);

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
    serializing::toBinary (state, block);
}

void ProcessorBase::LastSavedEditorSize::toValueTree (ValueTree& tree)
{
    const auto size = base.getSavedEditorSize();
    tree.setProperty ("editorSizeX", size.x, nullptr);
    tree.setProperty ("editorSizeY", size.y, nullptr);
}

void ProcessorBase::setStateInformation (const void* data, int size)
{
    serializing::fromBinary (data, size, state);
}

void ProcessorBase::LastSavedEditorSize::fromValueTree (const ValueTree& tree)
{
    base.saveEditorSize (tree.getProperty ("editorSizeX"),
                         tree.getProperty ("editorSizeY"));
    base.repaintEditor();
}

void ProcessorBase::processBlock (juce::AudioBuffer< float >& audio, juce::MidiBuffer& midi)
{
    processBlockInternal (audio, midi, floatEngine);
}

void ProcessorBase::processBlock (juce::AudioBuffer< double >& audio, juce::MidiBuffer& midi)
{
    processBlockInternal (audio, midi, doubleEngine);
}

void ProcessorBase::processBlockBypassed (juce::AudioBuffer< float >& audio, juce::MidiBuffer& midi)
{
    state.mainBypass->set (true);
    processBlockInternal (audio, midi, floatEngine);
}

void ProcessorBase::processBlockBypassed (juce::AudioBuffer< double >& audio, juce::MidiBuffer& midi)
{
    state.mainBypass->set (true);
    processBlockInternal (audio, midi, doubleEngine);
}

template < typename SampleType >
void ProcessorBase::processBlockInternal (juce::AudioBuffer< SampleType >& audio,
                                          juce::MidiBuffer&                midi,
                                          Engine< SampleType >&            engine)
{
    juce::ScopedNoDenormals nodenorms;

    const auto busLayout = getBusesLayout();
    const auto inBus     = findSubBuffer (busLayout, audio, true);
    auto       outBus    = findSubBuffer (busLayout, audio, false);

    engine.process (inBus, outBus, midi, state.mainBypass->get());
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


}  // namespace bav::dsp
