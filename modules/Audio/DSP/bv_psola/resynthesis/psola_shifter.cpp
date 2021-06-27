
namespace bav::dsp::psola
{
template < typename SampleType >
Shifter< SampleType >::Shifter (Analyzer& parentAnalyzer)
    : analyzer (parentAnalyzer), l (analyzer.getBroadcaster(), [&]
                                    { currentSample = 0; })
{
    while (grains.size() < 40)
        grains.add (new Grain (analyzer.getStorage()));
}

template < typename SampleType >
void Shifter< SampleType >::setPitch (float desiredFrequency, double samplerate)
{
    desiredPeriod = math::periodInSamples (samplerate, desiredFrequency);
}

template < typename SampleType >
void Shifter< SampleType >::getSamples (SampleType* outputSamples, int numSamples)
{
    for (int i = 0; i < numSamples; ++i)
        outputSamples[i] = getNextSample();
}

template < typename SampleType >
SampleType Shifter< SampleType >::getNextSample()
{
    jassert (desiredPeriod > 0);

    if (samplesToNextGrain == 0 || ! areAnyGrainsActive())
        startNewGrain();

    auto sample = SampleType (0);

    for (auto* grain : grains)
        if (grain->isActive())
            sample += grain->getNextSample();

    ++currentSample;
    --samplesToNextGrain;

    return sample;
}

template < typename SampleType >
void Shifter< SampleType >::startNewGrain()
{
    if (auto* grain = getAvailableGrain())
    {
        grain->startNewGrain (analyzer.getStartOfClosestGrain (currentSample),
                              analyzer.getGrainLength());
    }
    else
    {
        jassertfalse;
    }

    samplesToNextGrain = desiredPeriod;
}

template < typename SampleType >
SynthesisGrain< SampleType >* Shifter< SampleType >::getAvailableGrain() const
{
    for (auto* grain : grains)
        if (! grain->isActive())
            return grain;

    return nullptr;
}

template < typename SampleType >
bool Shifter< SampleType >::areAnyGrainsActive() const
{
    for (auto* grain : grains)
        if (grain->isActive())
            return true;

    return false;
}


template class Shifter< float >;
template class Shifter< double >;

}  // namespace bav::dsp::psola
