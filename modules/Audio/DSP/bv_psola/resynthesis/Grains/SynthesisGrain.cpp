
namespace bav::dsp::psola
{
template < typename SampleType >
SynthesisGrain< SampleType >::SynthesisGrain (const AnalysisGrainStorage< SampleType >& storageToUse)
    : storage (storageToUse)
{
}

template < typename SampleType >
bool SynthesisGrain< SampleType >::isActive() const
{
    return active;
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

    auto index = currentTick;

    if (currentTick > grainLength)
    {
        active = false;
        index  = grainLength;
    }

    return storage.getSample (startIndex, index) * getWindowValue (index);
}

template < typename FloatType >
static FloatType ncos (int order, int i, int size) noexcept
{
    return std::cos (static_cast< FloatType > (order * i)
                     * juce::MathConstants< FloatType >::pi / static_cast< FloatType > (size - 1));
}
template float  ncos (int, int, int) noexcept;
template double ncos (int, int, int) noexcept;

template < typename SampleType >
SampleType SynthesisGrain< SampleType >::getWindowValue (int index) const noexcept
{
    const auto cos2 = ncos< SampleType > (2, index, grainLength);
    return static_cast< SampleType > (0.5 - 0.5 * cos2);
}

template class SynthesisGrain< float >;
template class SynthesisGrain< double >;

}  // namespace bav::dsp::psola
