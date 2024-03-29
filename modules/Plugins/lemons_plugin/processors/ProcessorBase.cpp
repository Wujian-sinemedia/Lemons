/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

namespace lemons::plugin
{

ProcessorBase::ProcessorBase (dsp::Engine<float>&		 floatEngineToUse,
							  dsp::Engine<double>&		 doubleEngineToUse,
							  State&					 stateToUse,
							  const BusesProperties&	 busesLayout,
							  const ProcessorAttributes& attributes,
							  const Version&			 processorVersion)
	: BasicProcessor (busesLayout), floatEngine (floatEngineToUse), doubleEngine (doubleEngineToUse), state (stateToUse), processorAttributes (attributes.withDefaultBuses (busesPropertiesToValueTree (busesLayout))), version (processorVersion)
{
	state.parameters.addTo (*this);
}

ProcessorBase::ProcessorBase (dsp::Engine<float>&		 floatEngineToUse,
							  dsp::Engine<double>&		 doubleEngineToUse,
							  State&					 stateToUse,
							  const ProcessorAttributes& attributes)
	: ProcessorBase (floatEngineToUse, doubleEngineToUse, stateToUse, busesPropertiesFromValueTree (attributes.defaultBusesLayout), attributes)
{
}

void ProcessorBase::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	jassert (sampleRate > 0. && samplesPerBlock > 0);

	const auto numChannels = std::max (getTotalNumInputChannels(), getTotalNumOutputChannels());

	jassert (numChannels > 0);

	if (isUsingDoublePrecision())
	{
		floatEngine.releaseResources();
		floatProcessor.releaseResources();

		doubleEngine.prepare (sampleRate, samplesPerBlock, numChannels);
		doubleProcessor.prepare (samplesPerBlock);

		setLatencySamples (doubleEngine.reportLatency());
	}
	else
	{
		doubleEngine.releaseResources();
		doubleProcessor.releaseResources();

		floatEngine.prepare (sampleRate, samplesPerBlock, numChannels);
		floatProcessor.prepare (samplesPerBlock);

		setLatencySamples (floatEngine.reportLatency());
	}
}

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

	doubleProcessor.releaseResources();
	floatProcessor.releaseResources();
}

static constexpr auto processorVersionProperty = "ProcessorVersion";

ValueTree ProcessorBase::saveState (bool currentProgramOnly) const
{
	auto tree = state.saveToValueTree (currentProgramOnly);

	tree.setProperty (processorVersionProperty, version.toString(), nullptr);

	return tree;
}

void ProcessorBase::getStateInformation (juce::MemoryBlock& block)
{
	juce::MemoryOutputStream os { block, false };

	saveState (false).writeToStream (os);
}

void ProcessorBase::getCurrentProgramStateInformation (juce::MemoryBlock& block)
{
	juce::MemoryOutputStream os { block, false };

	saveState (true).writeToStream (os);
}


void ProcessorBase::loadState (const ValueTree& tree)
{
	if (tree.hasProperty (processorVersionProperty))
	{
		const auto loadedVersion = Version::fromString (tree.getProperty (processorVersionProperty).toString());

		if (loadedVersion != version)
		{
			DBG ("Processor version is " + version.toString() + "; version of loaded state is " + loadedVersion.toString());
		}
	}
	else
	{
		DBG ("Version property not present in loaded state!");
	}

	state.loadFromValueTree (tree);
}

void ProcessorBase::setStateInformation (const void* data, int size)
{
	loadState (ValueTree::readFromData (data, static_cast<size_t> (size)));

	callEditorMethod ([w = state.editorSize.getWidth(), h = state.editorSize.getHeight()] (juce::AudioProcessorEditor& e)
					  { e.setSize (w, h); });
}

void ProcessorBase::setCurrentProgramStateInformation (const void* data, int size)
{
	loadState (ValueTree::readFromData (data, static_cast<size_t> (size)));

	repaintEditor();
}


void ProcessorBase::processBlock (AudioBuffer<float>& audio, MidiBuffer& midi)
{
	const juce::ScopedNoDenormals nodenorms;

	floatProcessor.process (audio, midi);
}

void ProcessorBase::processBlock (AudioBuffer<double>& audio, MidiBuffer& midi)
{
	const juce::ScopedNoDenormals nodenorms;

	doubleProcessor.process (audio, midi);
}

void ProcessorBase::processBlockBypassed (AudioBuffer<float>& audio, MidiBuffer& midi)
{
	const juce::ScopedNoDenormals nodenorms;

	getBypass().set (true);

	floatProcessor.process (audio, midi);
}

void ProcessorBase::processBlockBypassed (AudioBuffer<double>& audio, MidiBuffer& midi)
{
	const juce::ScopedNoDenormals nodenorms;

	getBypass().set (true);

	doubleProcessor.process (audio, midi);
}


int ProcessorBase::getNumPrograms()
{
	return std::max (1, state.programs.numPrograms());
}

int ProcessorBase::getCurrentProgram()
{
	return 0;
}

void ProcessorBase::setCurrentProgram (int index)
{
	if (const auto* program = state.programs.getProgramAtIndex (index))
		loadState (program->state);
}

const String ProcessorBase::getProgramName (int index)
{
	if (const auto* program = state.programs.getProgramAtIndex (index))
		return TRANS (program->name);

	return TRANS ("Program");
}

void ProcessorBase::changeProgramName (int index, const String& newName)
{
	if (auto* program = state.programs.getProgramAtIndex (index))
		program->name = newName;
}

ToggleParameter& ProcessorBase::getBypass() const
{
	return state.parameters.getBypass();
}

juce::AudioProcessorParameter* ProcessorBase::getBypassParameter() const
{
	return &getBypass();
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

ProcessorAttributes ProcessorBase::getAttributes() const noexcept
{
	return processorAttributes;
}

Version ProcessorBase::getVersion() const noexcept
{
	return version;
}

/*-------------------------------------------------------------------------------------------------------------------------*/

template <typename SampleType>
ProcessorBase::InternalProcessor<SampleType>::InternalProcessor (dsp::Engine<SampleType>& engineToUse, ProcessorBase& processorBase)
	: engine (engineToUse), audioProcessor (processorBase), list (processorBase.getState().parameters)
{
}

template <typename SampleType>
bool ProcessorBase::InternalProcessor<SampleType>::shouldChopAroundMidiMessage (const juce::MidiMessage& m) const
{
	if (m.isAllNotesOff() || m.isAllSoundOff() || m.isProgramChange() || m.isResetAllControllers())
		return true;

	if (m.isController())
		return list.isControllerMapped (m.getControllerNumber());

	return false;
}

template <typename SampleType>
void ProcessorBase::InternalProcessor<SampleType>::handleMidiMessage (const juce::MidiMessage& m)
{
	jassert (shouldChopAroundMidiMessage (m));

	if (m.isAllNotesOff() || m.isAllSoundOff())
	{
		engine.reset();
		return;
	}

	if (m.isResetAllControllers())
	{
		list.resetAllControllerMappedParams();
		return;
	}

	if (m.isProgramChange())
	{
		audioProcessor.setCurrentProgram (m.getProgramChangeNumber());
		return;
	}

	jassert (m.isController());

	const auto number = m.getControllerNumber();

	jassert (list.isControllerMapped (number));

	list.processControllerMessage (number, m.getControllerValue());
}

template <typename SampleType>
void ProcessorBase::InternalProcessor<SampleType>::renderChunk (AudioBuffer<SampleType>& audio, MidiBuffer& midi)
{
	const auto busLayout = audioProcessor.getBusesLayout();

	const auto findSubBuffer = [&] (bool isInput) -> AudioBuffer<SampleType>
	{
		const auto channelSetIndex = [input = isInput, &busLayout]
		{
			const auto numBuses = input ? busLayout.inputBuses.size() : busLayout.outputBuses.size();

			for (auto i = 0; i < numBuses; ++i)
				if (! busLayout.getChannelSet (input, i).isDisabled())
					return i;

			return 0;
		}();

		return audioProcessor.getBusBuffer (audio, isInput, channelSetIndex);
	};

	const auto inBus  = findSubBuffer (true);
	auto	   outBus = findSubBuffer (false);

	engine.process (inBus, outBus, midi, list.getBypass().get());
}

template class ProcessorBase::InternalProcessor<float>;
template class ProcessorBase::InternalProcessor<double>;

}  // namespace lemons::plugin
