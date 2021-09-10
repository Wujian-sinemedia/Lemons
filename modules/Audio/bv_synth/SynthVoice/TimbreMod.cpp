
namespace bav::dsp::synth
{
template < typename SampleType >
TimbreMod< SampleType >::TimbreMod (const FX::FilterParams& filterParamsToUse, const float& gainValueToUse, const bool& filterToggleToUse)
    : filterParams (filterParamsToUse), gainValue (gainValueToUse), filterToggle (filterToggleToUse)
{
}

template < typename SampleType >
TimbreMod< SampleType >::TimbreMod (const typename SynthBase< SampleType >::TimbreModParams& modSource)
    : TimbreMod (modSource.filterParams, modSource.gain, modSource.filterToggle)
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
    if (filterToggle)
    {
        filter->setParams (filterParams);
        filter.process (audio, toggle);
    }

    gain.process (audio);
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
void TimbreMod< SampleType >::setToggle (bool shouldModBeOn)
{
    toggle = shouldModBeOn;

    if (toggle)
        gain.setGain (gainValue);
    else
        gain.setGain (1.f);
}

template class TimbreMod< float >;
template class TimbreMod< double >;

}  // namespace bav::dsp::synth
