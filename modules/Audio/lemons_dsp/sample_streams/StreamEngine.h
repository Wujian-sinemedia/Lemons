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

namespace lemons::dsp
{
/** @ingroup audio_engines
    A utility class that wraps a SampleStream into a self-contained dsp::Engine.
    This will instantiate and manage the appropriate stream type for you. \n
    @tparam SampleType The floating point type to be used for samples. Must be either float or double.
    @tparam StreamType A template class representing the kind of stream you want this engine to use; for example, osc::Sine. This should be un-specialized. This type must inherit from SampleStream.
    @see Oscillator, dsp::Engine
 */
template <typename SampleType, template <typename T> class StreamType,
          LEMONS_MUST_INHERIT_FROM (StreamType<SampleType>, SampleStream<SampleType>)>
class StreamEngine : public dsp::Engine<SampleType>
{
public:

	explicit StreamEngine() = default;

protected:
	/** The managed stream objects. */
	ConstructedArray<StreamType<SampleType>> streams;

private:
	void renderBlock (const AudioBuffer<SampleType>&,
	                  AudioBuffer<SampleType>& output,
	                  MidiBuffer&, bool isBypassed) final
	{
		const auto numSamples  = output.getNumSamples();
		const auto numChannels = output.getNumChannels();

		jassert (streams->size() == numChannels);

		output.clear();

		if (isBypassed)
		{
			for (auto* stream : streams)
				stream->skipSamples (numSamples);
		}
		else
		{
			for (int chan = 0; chan < numChannels; ++chan)
				streams[chan]->getSamples (output, chan);
		}
	}

	void prepared (int, double samplerate, int numChannels) override
	{
		streams.resize (numChannels);
	}
};

}  // namespace lemons::dsp
