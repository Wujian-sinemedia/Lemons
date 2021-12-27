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

#include <lemons_dsp/lemons_dsp.h>

namespace lemons::plugin
{
/** Base class for a plugin processor.
    This class manages two instances of a dsp::Engine and uses the appropriate one -- if the host uses double precision processing, the double engine will be used; otherwise, the float engine will be used.
    @see Processor, Engine, BasicProcessor
 */
class ProcessorBase : public BasicProcessor
{
public:
	/** Creates a processor.
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

	/** Returns the plugin's state object. */
	[[nodiscard]] State& getState() noexcept;

	/** Returns the ProcessorAttributes object. */
	[[nodiscard]] const ProcessorAttributes& getAttributes() const noexcept;

private:
	juce::AudioProcessorParameter* getBypassParameter() const final;

	void getStateInformation (juce::MemoryBlock& block) final;
	void getCurrentProgramStateInformation (juce::MemoryBlock& block) final;
	void setStateInformation (const void* data, int size) final;
	void setCurrentProgramStateInformation (const void* data, int size) final;

	void prepareToPlay (double sampleRate, int samplesPerBlock) final;

	void reset() final;
	void releaseResources() final;

	bool supportsDoublePrecisionProcessing() const final;

	void processBlock (AudioBuffer<float>& audio, MidiBuffer& midi) final;
	void processBlock (AudioBuffer<double>& audio, MidiBuffer& midi) final;
	void processBlockBypassed (AudioBuffer<float>& audio, MidiBuffer& midi) final;
	void processBlockBypassed (AudioBuffer<double>& audio, MidiBuffer& midi) final;

	template <typename SampleType1, typename SampleType2>
	void prepareToPlayInternal (double sampleRate, int samplesPerBlock, int numChannels,
	                            dsp::Engine<SampleType1>& activeEngine,
	                            dsp::Engine<SampleType2>& idleEngine);

	template <typename SampleType>
	void processInternal (dsp::Engine<SampleType>& engine,
	                      AudioBuffer<SampleType>& audio, MidiBuffer& midi);

	template <typename SampleType>
	void processChunk (dsp::Engine<SampleType>& engine,
	                   AudioBuffer<SampleType>& audio, MidiBuffer& midi,
	                   int startSample, int endSample);

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

	MidiBuffer scratchMidiBuffer;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProcessorBase)
};

}  // namespace lemons::plugin
