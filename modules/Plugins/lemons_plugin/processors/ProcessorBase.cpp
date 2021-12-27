/*
 ======================================================================================

 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.

 ======================================================================================
 */

namespace lemons::plugin
{

ProcessorBase::ProcessorBase (dsp::Engine<float>&        floatEngineToUse,
                              dsp::Engine<double>&       doubleEngineToUse,
                              State&                     stateToUse,
                              const BusesProperties&     busesLayout,
                              const ProcessorAttributes& attributes)
    : BasicProcessor (busesLayout)
    , floatEngine (floatEngineToUse)
    , doubleEngine (doubleEngineToUse)
    , state (stateToUse)
    , processorAttributes (attributes)
{
	state.addTo (*this);
}

void ProcessorBase::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	const auto numChannels = std::max (getTotalNumInputChannels(), getTotalNumOutputChannels());

	if (isUsingDoublePrecision())
		prepareToPlayInternal (sampleRate, samplesPerBlock, numChannels, doubleEngine, floatEngine);
	else
		prepareToPlayInternal (sampleRate, samplesPerBlock, numChannels, floatEngine, doubleEngine);
}

template <typename SampleType1, typename SampleType2>
void ProcessorBase::prepareToPlayInternal (double                    sampleRate,
                                           int                       samplesPerBlock,
                                           int                       numChannels,
                                           dsp::Engine<SampleType1>& activeEngine,
                                           dsp::Engine<SampleType2>& idleEngine)
{
	if (idleEngine.isInitialized())
		idleEngine.releaseResources();

	jassert (sampleRate > 0. && samplesPerBlock > 0 && numChannels > 0);

	activeEngine.prepare (sampleRate, samplesPerBlock, numChannels);

	setLatencySamples (activeEngine.reportLatency());
}

template void ProcessorBase::prepareToPlayInternal (double, int, int, dsp::Engine<float>&, dsp::Engine<double>&);
template void ProcessorBase::prepareToPlayInternal (double, int, int, dsp::Engine<double>&, dsp::Engine<float>&);

void ProcessorBase::reset()
{
	if (isUsingDoublePrecision())
		doubleEngine.reset();
	else
		floatEngine.reset();
}

void ProcessorBase::releaseResources()
{
	doubleEngine.releaseResources();
	floatEngine.releaseResources();
}

void ProcessorBase::getStateInformation (juce::MemoryBlock& block)
{
	juce::MemoryOutputStream os { block, false };

	state.saveToValueTree (false).writeToStream (os);
}

void ProcessorBase::setStateInformation (const void* data, int size)
{
	state.loadFromValueTree (ValueTree::readFromData (data, static_cast<size_t> (size)));

	callEditorMethod ([&] (juce::AudioProcessorEditor& e)
	                  { e.setSize (state.editorSize.getWidth(),
		                           state.editorSize.getHeight()); });
}

void ProcessorBase::getCurrentProgramStateInformation (juce::MemoryBlock& block)
{
	juce::MemoryOutputStream os { block, false };

	state.saveToValueTree (true).writeToStream (os);
}

void ProcessorBase::setCurrentProgramStateInformation (const void* data, int size)
{
	state.loadFromValueTree (ValueTree::readFromData (data, static_cast<size_t> (size)));

	repaintEditor();
}

void ProcessorBase::processBlock (AudioBuffer<float>& audio, MidiBuffer& midi)
{
	processInternal (floatEngine, audio, midi);
}

void ProcessorBase::processBlock (AudioBuffer<double>& audio, MidiBuffer& midi)
{
	processInternal (doubleEngine, audio, midi);
}

void ProcessorBase::processBlockBypassed (AudioBuffer<float>& audio, MidiBuffer& midi)
{
	state.bypass.set (true);

	processInternal (floatEngine, audio, midi);
}

void ProcessorBase::processBlockBypassed (AudioBuffer<double>& audio, MidiBuffer& midi)
{
	state.bypass.set (true);

	processInternal (doubleEngine, audio, midi);
}

template <typename SampleType>
void ProcessorBase::processInternal (dsp::Engine<SampleType>& engine, AudioBuffer<SampleType>& audio, MidiBuffer& midi)
{
	const juce::ScopedNoDenormals nodenorms;

	for (const auto& m : midi)
	{
		const auto message = m.getMessage();

		if (message.isAllNotesOff() || message.isAllSoundOff())
		{
			engine.reset();
		}
		else if (message.isProgramChange())
		{
			setCurrentProgram (message.getProgramChangeNumber());
		}
		else if (message.isController())
		{
			state.processControllerMessage (message.getControllerNumber(),
			                                message.getControllerValue());
		}
	}

	const auto busLayout = getBusesLayout();

	const auto findSubBuffer = [&] (bool isInput) -> AudioBuffer<SampleType>
	{
		const auto channelSetIndex = [&]() -> int
		{
			const auto numBuses = isInput ? busLayout.inputBuses.size() : busLayout.outputBuses.size();

			for (int i = 0; i < numBuses; ++i)
				if (! busLayout.getChannelSet (isInput, i).isDisabled())
					return i;

			return 0;
		}();

		return getBusBuffer (audio, isInput, channelSetIndex);
	};

	const auto inBus  = findSubBuffer (true);
	auto       outBus = findSubBuffer (false);

	engine.process (inBus, outBus, midi, state.bypass.get());
}

template void ProcessorBase::processInternal (dsp::Engine<float>&, AudioBuffer<float>&, MidiBuffer&);
template void ProcessorBase::processInternal (dsp::Engine<double>&, AudioBuffer<double>&, MidiBuffer&);

int ProcessorBase::getNumPrograms()
{
	return 1;
}

int ProcessorBase::getCurrentProgram()
{
	return 0;
}

void ProcessorBase::setCurrentProgram (int index)
{
	if (index < 0 || index >= getNumPrograms())
		return;
}

const String ProcessorBase::getProgramName (int index)
{
	return TRANS ("Program");
}

void ProcessorBase::changeProgramName (int index, const String& newName)
{
}

juce::AudioProcessorParameter* ProcessorBase::getBypassParameter() const
{
	return &state.bypass;
}

bool ProcessorBase::supportsDoublePrecisionProcessing() const
{
	return true;
}

bool ProcessorBase::acceptsMidi() const
{
	return processorAttributes.acceptsMidi;
}

bool ProcessorBase::producesMidi() const
{
	return processorAttributes.producesMidi;
}

bool ProcessorBase::supportsMPE() const
{
	return processorAttributes.supportsMPE;
}

bool ProcessorBase::isMidiEffect() const
{
	return processorAttributes.isMidiEffect;
}

const String ProcessorBase::getName() const
{
	return TRANS (processorAttributes.name);
}

juce::StringArray ProcessorBase::getAlternateDisplayNames() const
{
	return processorAttributes.alternateNames;
}

State& ProcessorBase::getState() noexcept
{
	return state;
}

const ProcessorAttributes& ProcessorBase::getAttributes() const noexcept
{
	return processorAttributes;
}

}  // namespace lemons::plugin
