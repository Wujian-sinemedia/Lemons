
namespace lemons::dsp::FX
{
template < typename SampleType >
EQ< SampleType >::EQ (const EQParams& params)
{
    setParams (params);
}

template < typename SampleType >
void EQ< SampleType >::process (AudioBuffer< SampleType >& audio)
{
    jassert (getNumBands() > 0);

    for (auto* filter : bands)
        filter->process (audio);
}

template < typename SampleType >
void EQ< SampleType >::prepare (double samplerate, int blocksize)
{
    for (auto* filter : bands)
        filter->prepare (samplerate, blocksize);

    lastSamplerate = samplerate;
    lastBlocksize  = blocksize;
}

template < typename SampleType >
void EQ< SampleType >::addBand (Band* newFilter)
{
    bands.add (newFilter);
    newFilter->prepare (lastSamplerate, lastBlocksize);
}

template < typename SampleType >
int EQ< SampleType >::getNumBands() const
{
    return bands.size();
}

template < typename SampleType >
Filter< SampleType >* EQ< SampleType >::getBand (int index)
{
    if (index > bands.size()) return nullptr;
    return &bands[index]->getFilter();
}

template < typename SampleType >
Filter< SampleType >* EQ< SampleType >::getBandOfType (FilterType type)
{
    for (auto* band : bands)
    {
        auto& filter = band->getFilter();

        if (filter.getFilterType() == type)
            return &filter;
    }

    return nullptr;
}

template < typename SampleType >
Filter< SampleType >* EQ< SampleType >::getBandAtFrequency (float freq)
{
    for (auto* band : bands)
    {
        auto& filter = band->getFilter();

        if (filter.getFilterFrequency() == freq)
            return &filter;
    }

    return nullptr;
}

template < typename SampleType >
EQParams EQ< SampleType >::getParams() const
{
    EQParams params;

    for (auto* band : bands)
        params.add (band->getFilter().getParams());

    return params;
}

template < typename SampleType >
void EQ< SampleType >::setParams (const EQParams& params)
{
    const auto numParams    = params.size();
    const auto prevNumBands = bands.size();

    for (int i = numParams; i < prevNumBands; ++i)
        bands.remove (i, true);

    for (int i = 0; i < numParams; ++i)
    {
        const auto filterParams = params.getUnchecked (i);

        if (i < prevNumBands)
        {
            bands[i]->getFilter().setParams (filterParams);
        }
        else
        {
            addBand (filterParams);
        }
    }

    jassert (bands.size() == numParams);
}

template class EQ< float >;
template class EQ< double >;

}  // namespace lemons::dsp::FX
