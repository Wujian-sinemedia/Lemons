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

template <typename T>
using AudioBuffer = juce::AudioBuffer<T>;

using juce::MidiBuffer;


/** @ingroup plugin_hosting
	A kind of dsp::Engine that holds a ProcessorHolder.
	@see ProcessorHolder, dsp::Engine
 */
template <typename SampleType>
class ProcessorEngine : public dsp::Engine<SampleType>
{
public:

	/** Creates an engine that will stream audio through the specified AudioProcessor. */
	explicit ProcessorEngine (juce::AudioProcessor& processor);

	/** Reports the latency in samples of the managed AudioProcessor.
		Note that this class is a dsp::Engine, and not a dsp::LatencyEngine; the AudioProcessor is expected to internally manage its own latency buffering if it requires any.
	*/
	[[nodiscard]] int reportLatency() const noexcept;

	/** The ProcessorHolder managing the AudioProcessor. */
	ProcessorHolder processorHolder;

private:

	void renderBlock (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midiMessages, bool isBypassed) final;

	void prepared (int blocksize, double samplerate, int numChannels) final;

	void released() final;

	AudioBuffer<SampleType> scratchBuffer;
};


/** @ingroup plugin_hosting
	A ProcessorEngine that instantiates a custom subclass of AudioProcessor and automatically creates a ProcessorEngine for you.
	@tparam ProcessorType The type of processor you wish to instantiate. This type must inherit from juce::AudioProcessor.
	@see ProcessorEngine
 */
template <typename SampleType, typename ProcessorType, LEMONS_MUST_INHERIT_FROM (ProcessorType, juce::AudioProcessor)>
struct TypedProcessorEngine final : public ProcessorEngine<SampleType>
{
	TypedProcessorEngine()
		: ProcessorEngine<SampleType> (typedProcessor)
	{
	}

	/** The managed processor. */
	ProcessorType typedProcessor;
};


template <typename SampleType, typename ProcessorType, typename... Args>
[[nodiscard]] std::unique_ptr<ProcessorEngine<SampleType>> createEngineForProcessor (Args&&... args)
{
	return std::make_unique<TypedProcessorEngine<SampleType, ProcessorType>> (std::forward<Args> (args)...);
}

}  // namespace lemons::plugin
