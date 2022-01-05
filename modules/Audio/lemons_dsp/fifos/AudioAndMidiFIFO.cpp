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
AudioAndMidiFIFO<SampleType>::AudioAndMidiFIFO (int samples)
{
	setSize (samples);
}

template <typename SampleType>
void AudioAndMidiFIFO<SampleType>::setSize (int numSamples, int numChannels)
{
	jassert (numSamples > 0 && numChannels > 0);

	clear();

	audio.resize (numSamples, numChannels);
	midi.setSize (numSamples);
}

template <typename SampleType>
void AudioAndMidiFIFO<SampleType>::push (const AudioBuffer<SampleType>& audioIn, const MidiBuffer& midiIn)
{
	audio.pushSamples (audioIn);
	midi.pushEvents (midiIn, audioIn.getNumSamples());

	jassert (audio.numStoredSamples() == midi.numStoredSamples());
}

template <typename SampleType>
void AudioAndMidiFIFO<SampleType>::pop (AudioBuffer<SampleType>& audioOut, MidiBuffer& midiOut)
{
	audio.popSamples (audioOut);
	midi.popEvents (midiOut, audioOut.getNumSamples());

	jassert (audio.numStoredSamples() == midi.numStoredSamples());
}

template <typename SampleType>
int AudioAndMidiFIFO<SampleType>::numStoredSamples() const noexcept
{
	jassert (audio.numStoredSamples() == midi.numStoredSamples());
	return audio.numStoredSamples();
}

template <typename SampleType>
void AudioAndMidiFIFO<SampleType>::clear()
{
	audio.clear();
	midi.clear();
}

template class AudioAndMidiFIFO<float>;
template class AudioAndMidiFIFO<double>;

}  // namespace lemons::dsp
