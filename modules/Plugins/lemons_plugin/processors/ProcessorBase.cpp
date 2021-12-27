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

	scratchMidiBuffer.ensureSize (static_cast<size_t> (samplesPerBlock));

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

	scratchMidiBuffer.clear();
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
	const auto findNextCriticalMidiMessage = [&] (juce::MidiBufferIterator it) -> juce::MidiBufferIterator
	{
		while (it != midi.cend())
		{
			++it;

			if (it == midi.cend())
				return it;

			const auto isCriticalMidiMessage = [&]() -> bool
			{
				const auto message = (*it).getMessage();

				if (message.isAllNotesOff() || message.isAllSoundOff() || message.isProgramChange())
					return true;

				if (message.isController())
					return state.isControllerMapped (message.getControllerNumber());

				return false;
			}();

			if (isCriticalMidiMessage)
				return it;
		}

		return midi.cend();
	};


	const juce::ScopedNoDenormals nodenorms;

	const auto numSamples = audio.getNumSamples();

	int  lastChunkEnd = 0;
	auto it           = findNextCriticalMidiMessage (midi.cbegin());

	while (it != midi.cend())
	{
		const auto meta      = *it;
		const auto message   = meta.getMessage();
		const auto timestamp = meta.samplePosition;

		if (timestamp > lastChunkEnd)
			processChunk (engine, audio, midi, lastChunkEnd, timestamp);

		if (message.isAllNotesOff() || message.isAllSoundOff())
		{
			engine.reset();
		}
		else if (message.isProgramChange())
		{
			setCurrentProgram (message.getProgramChangeNumber());
		}
		else
		{
			jassert (message.isController());

			const auto number = message.getControllerNumber();

			jassert (state.isControllerMapped (number));

			state.processControllerMessage (number, message.getControllerValue());
		}

		const auto next = findNextCriticalMidiMessage (it);

		const auto nextTimestamp = [&]() -> int
		{
			if (next == midi.cend())
				return numSamples;

			return (*next).samplePosition;
		}();

		if (nextTimestamp > timestamp)
			processChunk (engine, audio, midi, timestamp, nextTimestamp);

		lastChunkEnd = nextTimestamp;
		it           = next;
	}

	if (lastChunkEnd < numSamples)
		processChunk (engine, audio, midi, lastChunkEnd, numSamples);
}

template void ProcessorBase::processInternal (dsp::Engine<float>&, AudioBuffer<float>&, MidiBuffer&);
template void ProcessorBase::processInternal (dsp::Engine<double>&, AudioBuffer<double>&, MidiBuffer&);

template <typename SampleType>
void ProcessorBase::processChunk (dsp::Engine<SampleType>& engine, AudioBuffer<SampleType>& audio, MidiBuffer& midi,
                                  int startSample, int endSample)
{
	const auto numSamples = endSample - startSample;

	if (numSamples == 0)
		return;

	jassert (numSamples > 0);

	const midi::ScopedMidiBufferAlias midiAlias { midi, scratchMidiBuffer, startSample, numSamples };

	auto audioAlias = dsp::buffers::getAliasBuffer (audio, startSample, numSamples);

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

		return getBusBuffer (audioAlias, isInput, channelSetIndex);
	};

	const auto inBus  = findSubBuffer (true);
	auto       outBus = findSubBuffer (false);

	engine.process (inBus, outBus, scratchMidiBuffer, state.bypass.get());
}

template void ProcessorBase::processChunk (dsp::Engine<float>&, AudioBuffer<float>&, MidiBuffer&, int, int);
template void ProcessorBase::processChunk (dsp::Engine<double>&, AudioBuffer<double>&, MidiBuffer&, int, int);

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
