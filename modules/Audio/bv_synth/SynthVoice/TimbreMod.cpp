
namespace bav::dsp::synth
{
template < typename SampleType >
TimbreMod< SampleType >::TimbreMod (FX::FilterParams& filterParamsToUse)
    : filterParams (filterParamsToUse)
{
}

template < typename SampleType >
void TimbreMod< SampleType >::prepare (int blocksize, double samplerate)
{
    filter.prepare (samplerate, blocksize, 1);
    gain.prepare (samplerate, blocksize);
}

template < typename SampleType >
void TimbreMod< SampleType >::process (AudioBuffer< SampleType >& audio)
{
    gain.process (audio);
    filter->setParams (filterParams);
    filter.process (audio, toggle);
}

template < typename SampleType >
void TimbreMod< SampleType >::skipSamples (int numSamples)
{
    gain.skipSamples (numSamples);
}

template < typename SampleType >
void TimbreMod< SampleType >::reset()
{
    gain.reset();
}

template < typename SampleType >
void TimbreMod< SampleType >::setGain (float newGain)
{
    gain.setGain (newGain);
}

template < typename SampleType >
void TimbreMod< SampleType >::setToggle (bool shouldModBeOn)
{
    toggle = shouldModBeOn;
}

template class TimbreMod< float >;
template class TimbreMod< double >;

}  // namespace bav::dsp::synth
