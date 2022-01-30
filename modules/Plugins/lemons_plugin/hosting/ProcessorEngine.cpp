
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
 *  ======================================================================================
 */

namespace lemons::plugin
{

template <typename SampleType>
ProcessorEngine<SampleType>::ProcessorEngine (juce::AudioProcessor& processor)
	: processorHolder (processor)
{
	if constexpr (std::is_same_v<SampleType, double>)
	{
		jassert (processor.supportsDoublePrecisionProcessing());
	}
}

template <typename SampleType>
int ProcessorEngine<SampleType>::reportLatency() const noexcept
{
	return processorHolder->getLatencySamples();
}

template <typename SampleType>
void ProcessorEngine<SampleType>::renderBlock (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midiMessages, bool isBypassed)
{
	dsp::buffers::copy (input, scratchBuffer);

	if (isBypassed)
		processorHolder->processBlockBypassed (scratchBuffer, midiMessages);
	else
		processorHolder->processBlock (scratchBuffer, midiMessages);

	dsp::buffers::copy (scratchBuffer, output);
}

template <typename SampleType>
void ProcessorEngine<SampleType>::prepared (int blocksize, double samplerate, int numChannels)
{
	processorHolder.prepareForPlayback<SampleType> (samplerate, blocksize, numChannels);

	scratchBuffer.setSize (numChannels, blocksize, true, true, true);
}

template <typename SampleType>
void ProcessorEngine<SampleType>::released()
{
	processorHolder->releaseResources();
}

template class ProcessorEngine<float>;
template class ProcessorEngine<double>;

}  // namespace lemons::plugin
