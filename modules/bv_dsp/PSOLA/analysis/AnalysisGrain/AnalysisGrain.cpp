
namespace bav
{
template < typename SampleType >
AnalysisGrain< SampleType >::AnalysisGrain()
    : numActive (0), origStart (0), size (0), percentOfExpectedGrainSize (0)
{
}

template < typename SampleType >
void AnalysisGrain< SampleType >::reserveSize (int numSamples)
{
    samples.setSize (1, numSamples);
}

template < typename SampleType >
void AnalysisGrain< SampleType >::incNumActive() noexcept
{
    ++numActive;
}

template < typename SampleType >
void AnalysisGrain< SampleType >::decNumActive() noexcept
{
    --numActive;

    if (numActive <= 0) clear();
}

template < typename SampleType >
int AnalysisGrain< SampleType >::numReferences() const noexcept
{
    return numActive;
}

template < typename SampleType >
float AnalysisGrain< SampleType >::percentOfExpectedSize() const noexcept
{
    return percentOfExpectedGrainSize;
}

template < typename SampleType >
void AnalysisGrain< SampleType >::storeNewGrain (const SampleType* inputSamples, int startSample, int endSample, int expectedGrainSize)
{
    samples.clear();
    origStart = startSample;
    size      = endSample - startSample + 1;
    jassert (size > 0);
    jassert (samples.getNumSamples() >= size);
    jassert (expectedGrainSize > 0);
    percentOfExpectedGrainSize = float (size) / float (expectedGrainSize);

    auto* writing = samples.getWritePointer (0);
    vecops::copy (inputSamples + startSample, writing, size);

    //  apply Hann window to input samples
    for (int s = 0; s < size; ++s)
        writing[s] *= getWindowValue (size, s);
}

template < typename SampleType >
void AnalysisGrain< SampleType >::clear() noexcept
{
    samples.clear();
    size                       = 0;
    origStart                  = 0;
    numActive                  = 0;
    percentOfExpectedGrainSize = 0.0f;
}

template < typename SampleType >
SampleType AnalysisGrain< SampleType >::getSample (int index) const
{
    jassert (index < size);
    return samples.getSample (0, index);
}

template < typename SampleType >
int AnalysisGrain< SampleType >::getSize() const noexcept
{
    return size;
}

template < typename SampleType >
int AnalysisGrain< SampleType >::pitchMark() const noexcept
{
    return origStart;
}

template < typename SampleType >
bool AnalysisGrain< SampleType >::isEmpty() const noexcept
{
    return size < 1;
}

template < typename SampleType >
SampleType AnalysisGrain< SampleType >::getWindowValue (int windowSize, int index) const
{
    const auto cos2 =
        std::cos (static_cast< SampleType > (2 * index) * juce::MathConstants< SampleType >::pi / static_cast< SampleType > (windowSize - 1));

    return static_cast< SampleType > (0.5 - 0.5 * cos2);
}


template class AnalysisGrain< float >;
template class AnalysisGrain< double >;

}  // namespace bav
