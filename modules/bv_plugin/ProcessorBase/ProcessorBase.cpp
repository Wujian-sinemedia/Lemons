
namespace bav::dsp
{
ProcessorBase::ProcessorBase (ParameterList&  parameterList,
                              Engine<float>&  floatEngineToUse,
                              Engine<double>& doubleEngineToUse,
                              juce::AudioProcessor::BusesProperties busesLayout)
: BasicProcessorBase (busesLayout),
parameterProcessor (*this, parameterList),
floatEngine (floatEngineToUse),
doubleEngine (doubleEngineToUse)
{
    auto initSamplerate = getSampleRate();
    if (initSamplerate <= 0.0) initSamplerate = 44100.0;
    
    auto initBlockSize = getBlockSize();
    if (initBlockSize <= 0) initBlockSize = 512;
    
    prepareToPlay (initSamplerate, initBlockSize);
}


ProcessorBase::ParameterProcessor::ParameterProcessor (ProcessorBase& p, ParameterList& l)
: ParameterProcessorBase (l),
processor (p)
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
    if (! getMainBypass().get())
        getMainBypass().set (true);

    processBlockInternal (audio, midi);
}

void ProcessorBase::processBlockBypassed (juce::AudioBuffer< double >& audio, juce::MidiBuffer& midi)
{
    if (! getMainBypass().get())
        getMainBypass().set (true);

    processBlockInternal (audio, midi);
}

template < typename SampleType >
void ProcessorBase::processBlockInternal (juce::AudioBuffer< SampleType >& audio, juce::MidiBuffer& midi)
{
    juce::ScopedNoDenormals nodenorms;
    parameterProcessor.process (audio, midi);
}


void ProcessorBase::ParameterProcessor::renderChunk (juce::AudioBuffer< float >& audio, juce::MidiBuffer& midi)
{
    processor.renderChunk (processor.floatEngine, audio, midi);
}

void ProcessorBase::ParameterProcessor::renderChunk (juce::AudioBuffer< double >& audio, juce::MidiBuffer& midi)
{
    processor.renderChunk (processor.doubleEngine, audio, midi);
}


inline int getIndexOfFirstValidChannelSet (const juce::AudioProcessor::BusesLayout& busLayout, bool isInput)
{
    const auto numBuses = isInput ? busLayout.inputBuses.size() : busLayout.outputBuses.size();
    
    for (int i = 0; i < numBuses; ++i)
        if (! busLayout.getChannelSet (isInput, i).isDisabled())
            return i;
    
    return 0;
}

template<typename SampleType>
inline juce::AudioBuffer<SampleType> findSubBuffer (ProcessorBase& p,
                                                    juce::AudioBuffer<SampleType>& origBuffer,
                                                    bool isInput)
{
    return p.getBusBuffer (origBuffer, isInput,
                           getIndexOfFirstValidChannelSet (p.getBusesLayout(), isInput));
}


template<typename SampleType>
void ProcessorBase::renderChunk (Engine< SampleType >& engine, juce::AudioBuffer< SampleType >& audio, juce::MidiBuffer& midi)
{
    auto inBus  = findSubBuffer (*this, audio, true);
    auto outBus = findSubBuffer (*this, audio, false);
    
    engine.process (inBus, outBus, midi, getMainBypass().get());
}

}  // namespace bav::dsp
