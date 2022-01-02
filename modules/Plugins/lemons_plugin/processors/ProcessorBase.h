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

#pragma once


namespace lemons::plugin
{
/** Base class for a plugin processor.
    This class manages two instances of a dsp::Engine and uses the appropriate one -- if the host uses double precision processing, the double engine will be used; otherwise, the float engine will be used.
    @see Processor, Engine, BasicProcessor
 */
class ProcessorBase : public BasicProcessor
{
public:
	/** Creates a processor with an explicitly specified default BusesProperties.
	    @param floatEngineToUse The float specialization of the Engine to use.
	    @param doubleEngineToUse The double specialization of the Engine to use.
	    @param stateToUse The plugin state to reference.
	    @param busesLayout The buses layout to pass to the underlying juce::AudioProcessor.
	    @param attributes The ProcessorAttributes object to use.
	 */
	explicit ProcessorBase (dsp::Engine<float>&        floatEngineToUse,
	                        dsp::Engine<double>&       doubleEngineToUse,
	                        State&                     stateToUse,
	                        const BusesProperties&     busesLayout = getDefaultBusesLayout(),
	                        const ProcessorAttributes& attributes  = ProcessorAttributes::fromProjectDefines());

	/** Creates a processor with a BusesProperties object created from the ProcessorAttributes object. */
	explicit ProcessorBase (dsp::Engine<float>&        floatEngineToUse,
	                        dsp::Engine<double>&       doubleEngineToUse,
	                        State&                     stateToUse,
	                        const ProcessorAttributes& attributes);

	/** Returns the plugin's state object. */
	[[nodiscard]] State& getState() noexcept;

	/** Returns the plugin's bypass parameter. */
	[[nodiscard]] ToggleParameter& getBypass() const;

	/** Returns the current state of the ProcessorAttributes object. */
	[[nodiscard]] ProcessorAttributes getAttributes() const noexcept;

	/** Captures a snapshot of the processor's current state as a ValueTree. */
	[[nodiscard]] ValueTree saveState (bool currentProgramOnly = false) const;

	/** Reloads a previously saved state from a ValueTree. */
	void loadState (const ValueTree& tree);

private:
	juce::AudioProcessorParameter* getBypassParameter() const final;

	void getStateInformation (juce::MemoryBlock& block) final;
	void setStateInformation (const void* data, int size) final;
	void getCurrentProgramStateInformation (juce::MemoryBlock& block) final;
	void setCurrentProgramStateInformation (const void* data, int size) final;

	void prepareToPlay (double sampleRate, int samplesPerBlock) final;

	void reset() final;
	void releaseResources() final;

	bool supportsDoublePrecisionProcessing() const final;

	void processBlock (AudioBuffer<float>& audio, MidiBuffer& midi) final;
	void processBlock (AudioBuffer<double>& audio, MidiBuffer& midi) final;
	void processBlockBypassed (AudioBuffer<float>& audio, MidiBuffer& midi) final;
	void processBlockBypassed (AudioBuffer<double>& audio, MidiBuffer& midi) final;

	bool acceptsMidi() const final;
	bool producesMidi() const final;
	bool supportsMPE() const final;
	bool isMidiEffect() const final;

	const String      getName() const final;
	juce::StringArray getAlternateDisplayNames() const final;

	int          getNumPrograms() final;
	int          getCurrentProgram() final;
	void         setCurrentProgram (int index) final;
	const String getProgramName (int index) final;
	void         changeProgramName (int index, const String& newName) final;

	//    double getTailLengthSeconds() const final;

	dsp::Engine<float>&  floatEngine;
	dsp::Engine<double>& doubleEngine;

	State& state;

	const ProcessorAttributes processorAttributes;

	template <typename SampleType>
	class InternalProcessor : public midi::ChoppingProcessor<SampleType>
	{
	public:
		explicit InternalProcessor (dsp::Engine<SampleType>& engineToUse, ProcessorBase& processorBase);

	private:
		[[nodiscard]] bool shouldChopAroundMidiMessage (const juce::MidiMessage& m) const final;

		void handleMidiMessage (const juce::MidiMessage& m) final;

		void renderChunk (AudioBuffer<SampleType>& audio, MidiBuffer& midi) final;

		dsp::Engine<SampleType>& engine;

		juce::AudioProcessor& audioProcessor;

		ParameterList& list;
	};

	InternalProcessor<float>  floatProcessor { floatEngine, *this };
	InternalProcessor<double> doubleProcessor { doubleEngine, *this };

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProcessorBase)
};

}  // namespace lemons::plugin


#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

class ProcessorBaseTest : public AudioProcessorTestBase
{
public:
	ProcessorBaseTest();

private:

	plugin::State state;

	dsp::PassThroughEngine<float>  floatEngine;
	dsp::PassThroughEngine<double> doubleEngine;

	plugin::ProcessorBase processor { floatEngine, doubleEngine, state };
};

// static ProcessorBaseTest processorBaseTest;

}  // namespace lemons::tests

#endif
