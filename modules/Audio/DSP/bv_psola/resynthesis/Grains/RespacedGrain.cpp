
namespace bav::dsp::psola
{
template < typename SampleType >
RespacedGrain< SampleType >::RespacedGrain (const CircularBuffer< SampleType >& storageToUse)
    : grain (storageToUse)
{
}

template < typename SampleType >
bool RespacedGrain< SampleType >::isActive() const
{
    if (zeroesLeft > 0)
        return true;

    return grain.isActive();
}

template < typename SampleType >
bool RespacedGrain< SampleType >::isHalfwayThrough() const
{
    if (zeroesLeft > 0)
        return false;

    return grain.isHalfwayThrough();
}

template < typename SampleType >
SampleType RespacedGrain< SampleType >::getNextSample()
{
    if (zeroesLeft > 0)
    {
        --zeroesLeft;
        return (SampleType) 0.;
    }

    return grain.getNextSample();
}

template < typename SampleType >
void RespacedGrain< SampleType >::startNewGrain (int start, int length, int numZeroesFirst)
{
    zeroesLeft = numZeroesFirst;
    grain.startNewGrain (start, length);
}

template class RespacedGrain< float >;
template class RespacedGrain< double >;

}  // namespace bav::dsp::psola
