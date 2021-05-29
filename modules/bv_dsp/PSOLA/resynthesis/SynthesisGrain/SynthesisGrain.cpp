
namespace bav
{
template < typename SampleType >
SynthesisGrain< SampleType >::SynthesisGrain()
    : readingIndex (0), grain (nullptr), zeroesLeft (0)
{
}

template < typename SampleType >
AnalysisGrain< SampleType >* SynthesisGrain< SampleType >::orig() const noexcept
{
    return grain;
}

template < typename SampleType >
bool SynthesisGrain< SampleType >::isActive() const noexcept
{
    if (grain == nullptr) return false;

    return grain->getSize() > 0;
}

template < typename SampleType >
bool SynthesisGrain< SampleType >::isHalfwayThrough() const noexcept
{
    return samplesLeft() == juce::roundToInt (getSize() * 0.5f);
}

template < typename SampleType >
void SynthesisGrain< SampleType >::startNewGrain (Grain* newGrain, int samplesInFuture)
{
    jassert (newGrain != nullptr && ! newGrain->isEmpty());
    newGrain->incNumActive();
    grain = newGrain;

    readingIndex = 0;

    zeroesLeft = samplesInFuture;
}

template < typename SampleType >
SampleType SynthesisGrain< SampleType >::getNextSample()
{
    jassert (isActive() && ! grain->isEmpty());

    if (zeroesLeft > 0)
    {
        jassert (readingIndex == 0);
        --zeroesLeft;
        return SampleType (0);
    }

    const auto sample = grain->getSample (readingIndex++);

    if (readingIndex >= grain->getSize()) stop();

    return sample;
}

template < typename SampleType >
int SynthesisGrain< SampleType >::samplesLeft() const noexcept
{
    if (isActive()) return grain->getSize() - readingIndex + std::max (0, zeroesLeft);

    return 0;
}

template < typename SampleType >
int SynthesisGrain< SampleType >::silenceLeft() const noexcept
{
    return std::max (0, zeroesLeft);
}

template < typename SampleType >
int SynthesisGrain< SampleType >::getSize() const noexcept
{
    if (grain != nullptr) return grain->getSize();

    return 0;
}

template < typename SampleType >
void SynthesisGrain< SampleType >::stop() noexcept
{
    readingIndex = 0;
    zeroesLeft   = 0;
    halfIndex    = 0;

    if (grain != nullptr)
    {
        grain->decNumActive();
        grain = nullptr;
    }
}


template class SynthesisGrain< float >;
template class SynthesisGrain< double >;

}  // namespace bav
