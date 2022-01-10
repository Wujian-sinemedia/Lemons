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


namespace lemons::dsp
{

template <typename SampleType>
Protector<SampleType>::Protector (bool hardClip, CallbackFunc&& whenMuteTriggered)
    : isHardClipping (hardClip)
    , muteCallback (std::move (whenMuteTriggered))
{
}

template <typename SampleType>
bool Protector<SampleType>::isMuted() const noexcept
{
	return mute;
}

template <typename SampleType>
void Protector<SampleType>::forceMute() noexcept
{
	mute = true;
}

template <typename SampleType>
void Protector<SampleType>::resetMute() noexcept
{
	mute = false;
}

template <typename SampleType>
void Protector<SampleType>::released()
{
	mute = false;
    internalEngine.releaseResources();
}

template <typename SampleType>
void Protector<SampleType>::renderBlock (const AudioBuffer<SampleType>& input,
                                         AudioBuffer<SampleType>& output, MidiBuffer& midi, bool isBypassed)
{
	if (mute || isBypassed)
	{
		internalEngine.process (input, output, midi, true);
		return;
	}

	const auto numSamples = input.getNumSamples();

	const auto channelIsClipping = [&] (int chan) -> bool
	{
		const auto* samples = input.getReadPointer (chan);

		for (int s = 0; s < numSamples; ++s)
		{
			const auto sample = samples[s];

			if (std::isnan (sample) || std::isinf (sample))
				return true;

			if (isHardClipping)
				if (sample > SampleType (1) || sample < SampleType (-1))
					return true;
		}

		return false;
	};

	for (int chan = 0; chan < input.getNumChannels(); ++chan)
	{
		if (channelIsClipping (chan))
		{
			mute = true;
			break;
		}
	}

	if (mute)
		if (muteCallback)
			muteCallback (input);

	internalEngine.process (input, output, midi, mute);
}

template <typename SampleType>
void Protector<SampleType>::prepared (int blocksize, double samplerate, int numChannels)
{
    internalEngine.prepare (samplerate, blocksize, numChannels);
}


template <typename SampleType>
void Protector<SampleType>::InternalPassthruEngine::renderBlock (const AudioBuffer<SampleType>& input,
                                                                 AudioBuffer<SampleType>&       output,
                                                                 MidiBuffer&, bool isBypassed)
{
	if (isBypassed)
		output.clear();
	else
		dsp::buffers::copy (input, output);
}

template class Protector<float>;
template class Protector<double>;


LEMONS_CREATE_DEFAULT_ENGINE_FACTORY (Protector, "Protection")

}  // namespace lemons::dsp
