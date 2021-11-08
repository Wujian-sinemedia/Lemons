
namespace lemons::plugin
{
ProcessorBase::ProcessorBase (StateBase&                            stateToUse,
                              dsp::Engine<float>&                   floatEngineToUse,
                              dsp::Engine<double>&                  doubleEngineToUse,
                              juce::AudioProcessor::BusesProperties busesLayout)
    : BasicProcessorBase (busesLayout)
    , state (stateToUse)
    , floatEngine (*this, state, floatEngineToUse)
    , doubleEngine (*this, state, doubleEngineToUse)
{
}

void ProcessorBase::saveEditorSize (int width, int height)
{
	//state.dimensions = { width, height };
}

[[nodiscard]] juce::Point<int> ProcessorBase::getSavedEditorSize() const noexcept
{
	//return state.dimensions;
}

juce::AudioProcessorParameter* ProcessorBase::getBypassParameter() const
{
	//return state.mainBypass.get();
}

void ProcessorBase::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	if (isUsingDoublePrecision())
		prepareToPlayInternal (sampleRate, samplesPerBlock, doubleEngine, floatEngine);
	else
		prepareToPlayInternal (sampleRate, samplesPerBlock, floatEngine, doubleEngine);
}

template <typename SampleType1, typename SampleType2>
void ProcessorBase::prepareToPlayInternal (double                       sampleRate,
                                           int                          samplesPerBlock,
                                           InternalEngine<SampleType1>& activeEngine,
                                           InternalEngine<SampleType2>& idleEngine)
{
	if (idleEngine->isInitialized())
		idleEngine->releaseResources();

	jassert (sampleRate > 0 && samplesPerBlock > 0);

	activeEngine.prepareToPlay (sampleRate, samplesPerBlock);

	setLatencySamples (activeEngine->reportLatency());
}

void ProcessorBase::releaseResources()
{
	doubleEngine->releaseResources();
	floatEngine->releaseResources();
}

void ProcessorBase::getStateInformation (juce::MemoryBlock& block)
{
	serializing::toBinary (state, block);
}

void ProcessorBase::setStateInformation (const void* data, int size)
{
	serializing::fromBinary (data, size, state);

	if (auto* e = getActiveEditor())
		e->setSize (state.dimensions.x, state.dimensions.y);
}

void ProcessorBase::processBlock (AudioBuffer<float>& audio, MidiBuffer& midi)
{
	juce::ScopedNoDenormals nodenorms;

	floatEngine.process (audio, midi);
}

void ProcessorBase::processBlock (AudioBuffer<double>& audio, MidiBuffer& midi)
{
	juce::ScopedNoDenormals nodenorms;

	doubleEngine.process (audio, midi);
}

void ProcessorBase::processBlockBypassed (AudioBuffer<float>& audio, MidiBuffer& midi)
{
	juce::ScopedNoDenormals nodenorms;

	state.mainBypass->set (true);
	floatEngine.process (audio, midi);
}

void ProcessorBase::processBlockBypassed (AudioBuffer<double>& audio, MidiBuffer& midi)
{
	juce::ScopedNoDenormals nodenorms;

	state.mainBypass->set (true);
	doubleEngine.process (audio, midi);
}

}  // namespace lemons::plugin
