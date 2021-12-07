namespace lemons::plugin
{
template <typename SampleType>
ProcessorBase::InternalEngine<SampleType>::InternalEngine (juce::AudioProcessor&    processorToUse,
                                                           StateBase&               stateToUse,
                                                           dsp::Engine<SampleType>& engineToUse)
    :  // ParameterProcessor<SampleType> (stateToUse.getParameters())
    processor (processorToUse)
    , state (stateToUse)
    , engine (engineToUse)
{
}

template <typename SampleType>
dsp::Engine<SampleType>* ProcessorBase::InternalEngine<SampleType>::operator->() noexcept
{
	return &engine;
}

template <typename SampleType>
void ProcessorBase::InternalEngine<SampleType>::prepareToPlay (double samplerate, int maxBlocksize)
{
	ParameterProcessor<SampleType>::prepare (maxBlocksize);
	engine.prepare (samplerate, maxBlocksize);
}

static inline int getIndexOfFirstValidChannelSet (const juce::AudioProcessor::BusesLayout& busLayout,
                                                  bool                                     isInput)
{
	const auto numBuses = isInput ? busLayout.inputBuses.size() : busLayout.outputBuses.size();

	for (int i = 0; i < numBuses; ++i)
		if (! busLayout.getChannelSet (isInput, i).isDisabled())
			return i;

	return 0;
}

template <typename SampleType>
using AudioBuffer = juce::AudioBuffer<SampleType>;

template <typename SampleType>
inline AudioBuffer<SampleType> findSubBuffer (juce::AudioProcessor&                    processor,
                                              const juce::AudioProcessor::BusesLayout& busLayout,
                                              juce::AudioBuffer<SampleType>&           origBuffer,
                                              bool                                     isInput)
{
	return processor.getBusBuffer (origBuffer, isInput,
	                               getIndexOfFirstValidChannelSet (busLayout, isInput));
}
template AudioBuffer<float>  findSubBuffer (juce::AudioProcessor&, const juce::AudioProcessor::BusesLayout&, juce::AudioBuffer<float>&, bool);
template AudioBuffer<double> findSubBuffer (juce::AudioProcessor&, const juce::AudioProcessor::BusesLayout&, juce::AudioBuffer<double>&, bool);


template <typename SampleType>
void ProcessorBase::InternalEngine<SampleType>::renderChunk (juce::AudioBuffer<SampleType>& audio, MidiBuffer& midi)
{
	const auto busLayout = processor.getBusesLayout();
	const auto inBus     = findSubBuffer (processor, busLayout, audio, true);
	auto       outBus    = findSubBuffer (processor, busLayout, audio, false);

	engine.process (inBus, outBus, midi, false);  // state.mainBypass->get());
}

template class ProcessorBase::InternalEngine<float>;
template class ProcessorBase::InternalEngine<double>;

}  // namespace lemons::plugin
