
namespace bav::dsp::FX
{
template < typename SampleType >
EQ< SampleType >::EQ (int initNumFilters)
{
    setNumFilters (initNumFilters);
}

template < typename SampleType >
void EQ< SampleType >::process (juce::AudioBuffer< SampleType >& audio)
{
    jassert (getNumFilters() > 0);

    for (auto* filter : filters)
        filter->process (audio);
}

template < typename SampleType >
void EQ< SampleType >::prepare (double samplerate, int blocksize)
{
    for (auto* filter : filters)
        filter->prepare (samplerate, blocksize);

    lastSamplerate = samplerate;
    lastBlocksize  = blocksize;
}

template < typename SampleType >
void EQ< SampleType >::setNumFilters (int numFilters)
{
    while (filters.size() < numFilters)
        filters.add (new Filter);

    while (filters.size() > numFilters)
        filters.removeLast();

    prepare (lastSamplerate, lastBlocksize);
}

template < typename SampleType >
int EQ< SampleType >::getNumFilters() const
{
    return filters.size();
}

template < typename SampleType >
Filter< SampleType >* EQ< SampleType >::getFilter (int index)
{
    if (index > filters.size()) return nullptr;
    return filters[index];
}

template < typename SampleType >
Filter< SampleType >* EQ< SampleType >::getFilterOfType (typename Filter::FilterType type)
{
    for (auto* filter : filters)
        if (filter->getFilterType() == type)
            return filter;

    return nullptr;
}

template class EQ< float >;
template class EQ< double >;

}  // namespace bav::dsp::FX
