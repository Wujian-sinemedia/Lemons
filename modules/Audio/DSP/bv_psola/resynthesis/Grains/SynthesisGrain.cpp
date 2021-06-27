
namespace bav::dsp::psola
{
template < typename SampleType >
SynthesisGrain< SampleType >::SynthesisGrain (const CircularBuffer< SampleType >& storageToUse)
    : buffer (storageToUse)
{
}

template < typename SampleType >
bool SynthesisGrain< SampleType >::isActive() const
{
    return active;
}

template < typename SampleType >
bool SynthesisGrain< SampleType >::isHalfwayThrough() const
{
    return currentTick == (grainLength / 2);
}

template < typename SampleType >
void SynthesisGrain< SampleType >::startNewGrain (int start, int length)
{
    active      = true;
    startIndex  = start;
    grainLength = length;

    currentTick = 0;
}

template < typename SampleType >
SampleType SynthesisGrain< SampleType >::getNextSample()
{
    jassert (grainLength > 0);

    currentTick++;

    if (currentTick > grainLength)
        active = false;

    return buffer.getSample (buffer.clipValueToCapacity (startIndex + currentTick))
         * getWindowValue (std::min (currentTick, grainLength));
}

template < typename SampleType >
SampleType SynthesisGrain< SampleType >::getWindowValue (int index)
{
    //const auto length = grainLength;

    return 0;
}

template class SynthesisGrain< float >;
template class SynthesisGrain< double >;

}  // namespace bav::dsp::psola
