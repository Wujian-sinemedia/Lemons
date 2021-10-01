
namespace lemons::dsp::FX
{
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
void EQ< SampleType >::serialize (TreeReflector& ref)
{
    using Params = juce::Array< ValueTree >;

    auto getParams = [&]
    {
        Params params;

        for (auto* band : bands)
            params.add (serializing::toTree (band->getFilter()));

        return params;
    };

    auto setParams = [&] (Params& params)
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
                serializing::fromTree (filterParams, bands[i]->getFilter());
            }
            else
            {
                auto* band = new Band();
                serializing::fromTree (filterParams, band->getFilter());
                addBand (band);
            }
        }

        jassert (bands.size() == numParams);
    };

    ref.addLambdaSet< Params > ("Bands", getParams, setParams);
}

template class EQ< float >;
template class EQ< double >;

}  // namespace lemons::dsp::FX
