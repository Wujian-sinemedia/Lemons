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

	tree.setProperty (processorVersionProperty, processorAttributes.version.toString(), nullptr);

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

		if (loadedVersion != processorAttributes.version)
		{
			DBG ("Processor version is " + processorAttributes.version.toString() + "; version of loaded state is " + loadedVersion.toString());
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

	callEditorMethod ([&] (juce::AudioProcessorEditor& e)
	                  { e.setSize (state.editorSize.getWidth(),
		                           state.editorSize.getHeight()); });
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

	state.bypass.set (true);

	floatProcessor.process (audio, midi);
}

void ProcessorBase::processBlockBypassed (AudioBuffer<double>& audio, MidiBuffer& midi)
{
	const juce::ScopedNoDenormals nodenorms;

	state.bypass.set (true);

	doubleProcessor.process (audio, midi);
}

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

/*-------------------------------------------------------------------------------------------------------------------------*/

template <typename SampleType>
ProcessorBase::InternalProcessor<SampleType>::InternalProcessor (dsp::Engine<SampleType>& engineToUse, ProcessorBase& processorBase)
    : engine (engineToUse)
    , audioProcessor (processorBase)
    , state (processorBase.getState())
{
}

template <typename SampleType>
bool ProcessorBase::InternalProcessor<SampleType>::shouldChopAroundMidiMessage (const juce::MidiMessage& m) const
{
	if (m.isAllNotesOff() || m.isAllSoundOff() || m.isProgramChange() || m.isResetAllControllers())
		return true;

	if (m.isController())
		return state.isControllerMapped (m.getControllerNumber());

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
		state.resetAllControllerMappedParams();
		return;
	}

	if (m.isProgramChange())
	{
		audioProcessor.setCurrentProgram (m.getProgramChangeNumber());
		return;
	}

	jassert (m.isController());

	const auto number = m.getControllerNumber();

	jassert (state.isControllerMapped (number));

	state.processControllerMessage (number, m.getControllerValue());
}

template <typename SampleType>
void ProcessorBase::InternalProcessor<SampleType>::renderChunk (AudioBuffer<SampleType>& audio, MidiBuffer& midi)
{
	const auto busLayout = audioProcessor.getBusesLayout();

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

		return audioProcessor.getBusBuffer (audio, isInput, channelSetIndex);
	};

	const auto inBus  = findSubBuffer (true);
	auto       outBus = findSubBuffer (false);

	jassert (inBus.getNumChannels() == outBus.getNumChannels());

	engine.process (inBus, outBus, midi, state.bypass.get());
}

template class ProcessorBase::InternalProcessor<float>;
template class ProcessorBase::InternalProcessor<double>;

}  // namespace lemons::plugin


#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

ProcessorBaseTest::ProcessorBaseTest()
    : AudioProcessorTestBase (processor, "ProcessorBase")
{
}

}  // namespace lemons::tests

#endif
