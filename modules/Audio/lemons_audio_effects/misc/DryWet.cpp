
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

namespace lemons::dsp::FX
{
template <typename SampleType>
DryWetMixer<SampleType>::DryWetMixer()
{
	mixer.setMixingRule (juce::dsp::DryWetMixingRule::balanced);
	setWetLatency (0);
}

template <typename SampleType>
DryWetMixer<SampleType>::DryWetMixer (int wetPercentMix, int wetLatencySamples)
{
	mixer.setMixingRule (juce::dsp::DryWetMixingRule::balanced);
	setWetMix (wetPercentMix);
	setWetLatency (wetLatencySamples);
}

template <typename SampleType>
void DryWetMixer<SampleType>::setWetMix (int wetPercent)
{
	mixer.setWetMixProportion (static_cast<SampleType> (wetPercent) * SampleType (0.01));
}

template <typename SampleType>
void DryWetMixer<SampleType>::setWetLatency (int latencySamples)
{
	mixer.setWetLatency (static_cast<SampleType> (latencySamples));
}

template <typename SampleType>
void DryWetMixer<SampleType>::prepare (int numChannels, int blocksize, double samplerate)
{
	spec.numChannels	  = static_cast<juce::uint32> (numChannels);
	spec.maximumBlockSize = static_cast<juce::uint32> (blocksize);
	spec.sampleRate		  = samplerate;

	mixer.prepare (spec);
}

template <typename SampleType>
void DryWetMixer<SampleType>::reset()
{
	mixer.reset();
}

template <typename SampleType>
void DryWetMixer<SampleType>::pushDrySamples (AudioBuffer<SampleType>& buffer)
{
	mixer.pushDrySamples (juce::dsp::AudioBlock<SampleType> (buffer));
}

template <typename SampleType>
void DryWetMixer<SampleType>::mixWetSamples (AudioBuffer<SampleType>& buffer)
{
	mixer.mixWetSamples (juce::dsp::AudioBlock<SampleType> (buffer));
}

template <typename SampleType>
void DryWetMixer<SampleType>::process (AudioBuffer<SampleType>& dry, AudioBuffer<SampleType>& wet)
{
	pushDrySamples (dry);
	mixWetSamples (wet);
}

template class DryWetMixer<float>;
template class DryWetMixer<double>;

}  // namespace lemons::dsp::FX
