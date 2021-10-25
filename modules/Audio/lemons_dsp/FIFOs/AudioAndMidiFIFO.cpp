
namespace lemons::dsp
{
template <typename SampleType>
AudioAndMidiFIFO<SampleType>::AudioAndMidiFIFO (int samples)
{
	setSize (samples);
}

template <typename SampleType>
void AudioAndMidiFIFO<SampleType>::setSize (int numSamples)
{
	audio.setMaximumSize (numSamples);
	midi.setSize (numSamples);
}

template <typename SampleType>
void AudioAndMidiFIFO<SampleType>::push (const AudioBuffer<SampleType>& audioIn, const MidiBuffer& midiIn)
{
	audio.pushSamples (audioIn);
	midi.pushEvents (midiIn, audioIn.getNumSamples());
}

template <typename SampleType>
void AudioAndMidiFIFO<SampleType>::pop (AudioBuffer<SampleType>& audioOut, MidiBuffer& midiOut)
{
	audio.popSamples (audioOut);
	midi.popEvents (midiOut, audioOut.getNumSamples());
}

template <typename SampleType>
int AudioAndMidiFIFO<SampleType>::numStoredSamples() const
{
	return audio.numStoredSamples();
}

template class AudioAndMidiFIFO<float>;
template class AudioAndMidiFIFO<double>;

}  // namespace lemons::dsp
