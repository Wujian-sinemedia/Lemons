
namespace bav::dsp::FX
{
template < typename SampleType >
DryWetMixer< SampleType >::DryWetMixer()
{
    mixer.setMixingRule (juce::dsp::DryWetMixingRule::balanced);
    setWetLatency (0);
}

template < typename SampleType >
void DryWetMixer< SampleType >::setWetMix (int wetPercent)
{
    mixer.setWetMixProportion (SampleType (wetPercent) * SampleType (0.01));
}

template < typename SampleType >
void DryWetMixer< SampleType >::setWetLatency (int latencySamples)
{
    mixer.setWetLatency (SampleType (latencySamples));
}

template < typename SampleType >
void DryWetMixer< SampleType >::prepare (int numChannels, int blocksize, double samplerate)
{
    spec.numChannels      = juce::uint32 (numChannels);
    spec.maximumBlockSize = juce::uint32 (blocksize);
    spec.sampleRate       = samplerate;

    mixer.prepare (spec);
}

template < typename SampleType >
void DryWetMixer< SampleType >::reset()
{
    mixer.reset();
}

template < typename SampleType >
void DryWetMixer< SampleType >::pushDrySamples (AudioBuffer& buffer)
{
    mixer.pushDrySamples (juce::dsp::AudioBlock< SampleType > (buffer));
}

template < typename SampleType >
void DryWetMixer< SampleType >::mixWetSamples (AudioBuffer& buffer)
{
    mixer.mixWetSamples (juce::dsp::AudioBlock< SampleType > (buffer));
}

template < typename SampleType >
void DryWetMixer< SampleType >::process (AudioBuffer& dry, AudioBuffer& wet)
{
    pushDrySamples (dry);
    mixWetSamples (wet);
}

template class DryWetMixer< float >;
template class DryWetMixer< double >;

}  // namespace bav::dsp::FX
