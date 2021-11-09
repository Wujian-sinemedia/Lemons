#pragma once

#include <lemons_dsp/lemons_dsp.h>
#include <lemons_core/lemons_core.h>
#include <lemons_plugin_parameters/lemons_plugin_parameters.h>

namespace lemons::plugin
{
/** Base class for a plugin processor.
    This class manages two instances of a dsp::Engine and uses the appropriate one -- if the host uses double precision processing, the double engine will be used; otherwise, the float engine will be used. \n \n
    This class is a SystemInitializer, meaning it sets up translations for you. This class also holds a ParameterProcessor internally.
    @see SystemInitializer, dsp::Engine, dsp::BasicProcessorBase, StateBase
 */
class ProcessorBase : public dsp::BasicProcessorBase
    , private SystemInitializer
{
    using MidiBuffer = juce::MidiBuffer;
    
    template<typename SampleType>
    using AudioBuffer = juce::AudioBuffer<SampleType>;
    
public:
	/** Creates a processor.
	    @param stateToUse The state object to reference. The internal ParameterProcessor will reference stateToUse.getParameters().
	    @param floatEngineToUse The float specialization of the dsp::Engine to use.
	    @param doubleEngineToUse The double specialization of the dsp::Engine to use.
	    @param busesLayout The buses layout to pass to the underlying juce::AudioProcessor.
	 */
	ProcessorBase (StateBase&                            stateToUse,
	               dsp::Engine<float>&                   floatEngineToUse,
	               dsp::Engine<double>&                  doubleEngineToUse,
	               juce::AudioProcessor::BusesProperties busesLayout = BusesProperties()
	                                                                       .withInput (TRANS ("Input"), juce::AudioChannelSet::stereo(), true)
	                                                                       .withOutput (TRANS ("Output"), juce::AudioChannelSet::stereo(), true));

	/** Saves the editor's current dimensions in the plugin state. */
	void saveEditorSize (int width, int height);

	/** Returns the state's saved editor dimensions. */
	[[nodiscard]] juce::Point<int> getSavedEditorSize() const noexcept;

private:
	juce::AudioProcessorParameter* getBypassParameter() const final;

	void getStateInformation (juce::MemoryBlock& block) override;
	void setStateInformation (const void* data, int size) override;

	void prepareToPlay (double sampleRate, int samplesPerBlock) final;

	void releaseResources() final;

	bool supportsDoublePrecisionProcessing() const final { return true; }

	void processBlock (AudioBuffer<float>& audio, MidiBuffer& midi) final;
	void processBlock (AudioBuffer<double>& audio, MidiBuffer& midi) final;
	void processBlockBypassed (AudioBuffer<float>& audio, MidiBuffer& midi) final;
	void processBlockBypassed (AudioBuffer<double>& audio, MidiBuffer& midi) final;

	/*-------------------------------------------------------*/

	template <typename SampleType>
	class InternalEngine : public ParameterProcessor<SampleType>
	{
	public:
		InternalEngine (juce::AudioProcessor&    processorToUse,
		                StateBase&               stateToUse,
		                dsp::Engine<SampleType>& engineToUse);

		void prepareToPlay (double samplerate, int maxBlocksize);

		dsp::Engine<SampleType>* operator->() noexcept;

	private:
		void renderChunk (juce::AudioBuffer<SampleType>& audio, MidiBuffer& midi) final;

		juce::AudioProcessor&    processor;
		StateBase&               state;
		dsp::Engine<SampleType>& engine;
	};

	/*-------------------------------------------------------*/

	template <typename SampleType1, typename SampleType2>
	void prepareToPlayInternal (double sampleRate, int samplesPerBlock,
	                            InternalEngine<SampleType1>& activeEngine,
	                            InternalEngine<SampleType2>& idleEngine);

	/*-------------------------------------------------------*/

	StateBase& state;

	InternalEngine<float>  floatEngine;
	InternalEngine<double> doubleEngine;
};

}  // namespace lemons::plugin
